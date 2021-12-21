/**
 * Power Management Process
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER_PROCESS_H
#define _PWRCONSUMER_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "pwrload_mngmnt.h"
#include "ecosense_cfg.h"

#include "LinkedList/LinkedList.h"

/**
 * @brief Power management process: controls state and then ready - starts child processes, which do the work
 */
class PwrConsumerProcess: public IFirmwareProcess {
	public:
		enum WorkState {
			NONE,
			START,
			ACTIVE,
			DONE
		};

	protected:
		struct TaskInfo {
			uint16_t	prcId;
			WorkState	state;
		};

        byte        	keyPin;
        uint32_t    	poweredTime;
		//WorkState		tasksArr[MAXTASKCOUNT];
		//const uint16_t	*taskIdList;
		//byte			taskCnt;
		LinkedList<TaskInfo*> tasks;

	public:
		//@implement
		PwrConsumerProcess(byte keyPin, IProcessMessage* msg): IFirmwareProcess(msg) {
			/*this->taskIdList = idList;
			this->taskCnt = tasks;*/
			this->keyPin = keyPin;
			this->poweredTime = 0;
		}

		//@implement
		void addTask(uint16_t prcId) {
			if (this->findTask(prcId) == -1) {
				TaskInfo* task = new TaskInfo();
				task->prcId = prcId;
				task->state = NONE;
				tasks.add(task);
			}
		}

		//@implement
		int findTask(uint16_t id) {
			for (byte i = 0; i < this->tasks.size(); i++) {
				if (this->tasks.get(i)->prcId == id) {
					return i;
				}
			}
			return -1;
		}

		//@implement
		void taskDone(uint16_t process_id) {
			int pos = this->findTask(process_id);
			if (pos == -1) return;

			this->tasks.get(pos)->state = DONE;
			this->getHost()->stopProcess(process_id);
		}

		/**
		 * This should be overriden by handler with logic
		 */
		virtual bool handleMessageLogic(IProcessMessage* msg) = 0;

        //@implement
		//@include "processy_cfg.h"
		//@include "pwrload_mngmnt.h"
		unsigned long run(unsigned long start) {
			if (this->poweredTime == 0) {
				this->poweredTime = PowerloadManagement::get()->requestPin(this->keyPin);
				// bit sleep - if got power - physical change state, if not - simple wait delay
				this->pause(10);
				return millis();
			}
			return IFirmwareProcess::run(start);
		}

		//@implement
		//@include "ecosense_messages.h"
		void update(unsigned long ms) {
			// we got POWER! ))

			switch (this->getWorkState())
			{
				case START: {
					TRACELNF("PwrConsumerProcess: start child processes");
					for (byte i = 0; i < this->tasks.size(); i++) {
						TaskInfo* task = this->tasks.get(i);
						this->getHost()->addProcess(task->prcId);
						task->state = ACTIVE;
					}
					return;
				}
				case DONE: {
					// shutdown
					TRACELNF("PwrConsumerProcess: shut down");
					//this->clearState();
					
					// unlock pwr key
					this->releaseLoad();
					this->sendMessage(ProcessOrderMessage::goNextOf(this->getId()));
					return;
				}
				default: {
					// no work should be done here?
					this->pause(100);
				}
			}
		}

		//@implement
		//@include "ecosense_messages.h"
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case TASKDONE_MESSAGE: {
					TRACELNF("PwrConsumerProcess/TASKDONE_MESSAGE")
					this->taskDone(((TaskDoneMessage*)msg)->getTaskId());
					return false;
				}
				case PRC_ORDER_MESSAGE: {
					if (((ProcessOrderMessage*)msg)->getNextId() != this->getId()) {
						ProcessOrderMessage* msg = ProcessOrderMessage::goNextOf(this->getId());
						this->getHost()->addProcess(msg->getNextId());	// start next of process list
						this->stop();
					}
					return false;
				}
			}
			if (this->getWorkState() != ACTIVE) return false;//deepSleep || 
			return this->handleMessageLogic(msg);
		}

		PwrConsumerProcess::WorkState getWorkState() {
			byte none = 0;
			byte done = 0;

			for (byte i = 0; i < this->tasks.size(); i++) {
				WorkState s = this->tasks.get(i)->state;
				if (s == NONE) {
					none++;
				} else if (s == DONE) {
					done++;
				}
			}
			if (none == this->tasks.size()) {
				return START;
			}
			if (done == this->tasks.size()) {
				return DONE;
			}
			return ACTIVE;
		}

        uint32_t getPoweredTime() {
            return this->poweredTime;
        }

        //@implement
        void releaseLoad() {
            if (this->poweredTime != 0) {
				if (!PowerloadManagement::get()->releasePin(this->poweredTime)) {
					TRACELNF("PwrConsumerProcess//got FALSE");
				}
				this->poweredTime = 0;
			}
        }

		//@implement
		~PwrConsumerProcess() {
			// stop process
            this->releaseLoad();

			for (int i = this->tasks.size()-1; i >= 0; i--) {
				delete this->tasks.remove(i);
			}
			TRACELNF("PwrConsumerProcess::stop");
		}
};

#endif