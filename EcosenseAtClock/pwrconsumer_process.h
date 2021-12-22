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

		/* Returns next consumer process id or 0 to stop */
		virtual uint16_t getNextConsumerId() = 0;

		PwrConsumerProcess(byte keyPin, IProcessMessage* msg);

		void addTask(uint16_t prcId);

		int findTask(uint16_t id);

		void taskDone(uint16_t process_id);

		/**
		 * This should be overriden by handler with logic
		 */
		virtual bool handleMessageLogic(IProcessMessage* msg) = 0;

		unsigned long run(unsigned long start);

		void update(unsigned long ms);

		bool handleMessage(IProcessMessage* msg);

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

        void releaseLoad();
};

#endif