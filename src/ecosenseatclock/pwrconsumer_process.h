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

		TaskInfo*		tasks[MAXTASKCOUNT];
		byte			tasksCnt = 0;

	public:

		/* Returns next consumer process id or 0 to stop */
		virtual uint16_t getNextConsumerId() = 0;

		//@implement
		PwrConsumerProcess(byte keyPin, IProcessMessage* msg): IFirmwareProcess(msg) {
			this->keyPin = keyPin;
			this->poweredTime = 0;
		}

		virtual ~PwrConsumerProcess() {
			for (byte i = 0; i < tasksCnt; i++) {
				delete tasks[i];
			}
		}

		//@implement
		void addTask(uint16_t prcId) {
			TaskInfo* t = new TaskInfo();
			t->prcId = prcId;
			t->state = NONE;
			tasks[tasksCnt] = t;
			tasksCnt++;
		}

		PwrConsumerProcess::TaskInfo* findTask(uint16_t id) {
			for (byte i = 0; i < tasksCnt; i++) {
				TaskInfo* t = this->tasks[i];
				if (t->prcId == id) {
					return t;
				}
			}
			return NULL;
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
					TRACELNF("PwrConsumer: start tasks");
					for (byte i = 0; i < tasksCnt; i++) {
						TaskInfo* t = tasks[i];
						this->getHost()->addProcess(t->prcId);
						t->state = ACTIVE;
					}
					return;
				}
				case DONE: {
					this->stop();

					// unlock pwr key
					this->releaseLoad();

					this->getHost()->addProcess(this->getNextConsumerId());	// start next of process list
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
					TaskInfo* t = this->findTask(((TaskDoneMessage*)msg)->getTaskId());
					if (!t) {
						TRACELNF("TASKDONE ERROR: id not found!")
						return;
					}

					t->state = DONE;
					this->getHost()->stopProcess(t->prcId);
					return false;
				}
			}
			if (this->getWorkState() != ACTIVE) return false;
			return this->handleMessageLogic(msg);
		}

		PwrConsumerProcess::WorkState getWorkState() {
			byte none = 0;
			byte done = 0;

			for (byte i = 0; i < tasksCnt; i++) {
				WorkState s = this->tasks[i]->state;
				if (s == NONE) {
					none++;
				} else if (s == DONE) {
					done++;
				}
			}
			if (none == tasksCnt) {
				return START;
			}
			if (done == tasksCnt) {
				return DONE;
			}
			return ACTIVE;
		}

        //@implement
        void releaseLoad() {
            if (this->poweredTime != 0) {
				if (!PowerloadManagement::get()->releasePin(this->poweredTime)) {
					TRACELNF("PwrConsumer: err on stop");
				}
				this->poweredTime = 0;
			}
        }
};

#endif