#include "pwrconsumer_process.h"
#include "processy_cfg.h"
#include "pwrload_mngmnt.h"
#include "ecosense_messages.h"

PwrConsumerProcess::PwrConsumerProcess(byte keyPin, IProcessMessage* msg) : IFirmwareProcess(msg){
	this->keyPin = keyPin;
	this->poweredTime = 0;
}

void PwrConsumerProcess::addTask(uint16_t prcId) {
	TaskInfo* t = new TaskInfo();
	t->prcId = prcId;
	t->state = NONE;
	tasks[tasksCnt] = t;
	tasksCnt++;
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

bool PwrConsumerProcess::handleMessage(IProcessMessage* msg) {
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

void PwrConsumerProcess::releaseLoad() {
    if (this->poweredTime != 0) {
				if (!PowerloadManagement::get()->releasePin(this->poweredTime)) {
					TRACELNF("PwrConsumer: err on stop");
				}
				this->poweredTime = 0;
			}
}
