#include "pwrconsumer_process.h"
#include "processy_cfg.h"
#include "pwrload_mngmnt.h"
#include "ecosense_messages.h"

PwrConsumerProcess::PwrConsumerProcess(byte keyPin, IProcessMessage* msg) : IFirmwareProcess(msg){
	/*this->taskIdList = idList;
	this->taskCnt = tasks;*/
	this->keyPin = keyPin;
	this->poweredTime = 0;
}

void PwrConsumerProcess::addTask(uint16_t prcId) {
	tasks[tasksCnt].prcId = prcId;
	tasks[tasksCnt].state = NONE;
	tasksCnt++;
}

int PwrConsumerProcess::findTask(uint16_t id) {
	for (byte i = 0; i < tasksCnt; i++) {
		if (this->tasks[i].prcId == id) {
			return i;
		}
	}
	return -1;
}

unsigned long PwrConsumerProcess::run(unsigned long start) {
	if (this->poweredTime == 0) {
		this->poweredTime = PowerloadManagement::get()->requestPin(this->keyPin);
		// bit sleep - if got power - physical change state, if not - simple wait delay
		this->pause(10);
		return millis();
	}
	return IFirmwareProcess::run(start);
}

void PwrConsumerProcess::update(unsigned long ms) {
	// we got POWER! ))
	switch (this->getWorkState())
	{
		case START: {
			TRACELNF("PwrConsumer: start tasks");
			for (byte i = 0; i < tasksCnt; i++) {
				//TaskInfo* task = this->tasks.get(i);
				this->getHost()->addProcess(tasks[i].prcId);
				tasks[i].state = ACTIVE;
				//task->state = ACTIVE;
			}
			return;
		}
		case DONE: {
			// shutdown
			TRACELNF("PwrConsumer: stop");
			
			this->stop();
			// unlock pwr key
			this->releaseLoad();
			uint16_t nextId = this->getNextConsumerId();
			if (nextId > 0) {
				this->getHost()->addProcess(nextId);	// start next of process list
			}
			return;
		}
		default: {
			// no work should be done here?
			this->pause(100);
		}
	}
}

bool PwrConsumerProcess::handleMessage(IProcessMessage* msg) {
	switch (msg->getType())
	{
		case TASKDONE_MESSAGE: {
			TRACELNF("PwrConsumer: task done")
			uint16_t taskId = ((TaskDoneMessage*)msg)->getTaskId();
			int pos = this->findTask(taskId);
			if (pos == -1) return;
			this->tasks[pos].state = DONE;
			this->getHost()->stopProcess(taskId);
			return false;
		}
	}
	if (this->getWorkState() != ACTIVE) return false;
	return this->handleMessageLogic(msg);
}

void PwrConsumerProcess::releaseLoad() {
    if (this->poweredTime != 0) {
				if (!PowerloadManagement::get()->releasePin(this->poweredTime)) {
					TRACELNF("PwrConsumer: err on stop");
				}
				this->poweredTime = 0;
			}
}
