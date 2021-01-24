#include "pwrconsumer_process.h"
#include "processy_cfg.h"
#include "pwrload_mngmnt.h"
#include "ecosense_messages.h"

PwrConsumerProcess::PwrConsumerProcess(byte keyPin, const uint16_t *idList, byte tasks, int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	TRACELNF("PwrConsumerProcess::init");
	this->taskIdList = idList;
	this->deepSleep = true;
	this->taskCnt = tasks;
	this->keyPin = keyPin;
	this->poweredTime = 0;
	this->clearState();
}

void PwrConsumerProcess::clearState() {
	for (byte i = 0; i < this->taskCnt; i++) {
		this->tasksArr[i] = NONE;
	}
}

int PwrConsumerProcess::findProcessId(uint16_t id) {
	for (byte i = 0; i < this->taskCnt; i++) {
		if (this->taskIdList[i] == id) {
			return i;
		}
	}
	return -1;
}

void PwrConsumerProcess::taskDone(uint16_t process_id) {
	int pos = this->findProcessId(process_id);
	if (pos == -1) return;
	this->tasksArr[pos] = DONE;
	this->getHost()->stopProcess(process_id);
}

bool PwrConsumerProcess::isPaused(unsigned long start) {
	if (deepSleep) {
		return true;
	}
	return IFirmwareProcess::isPaused(start);
}

unsigned long PwrConsumerProcess::run(unsigned long start) {
	if (deepSleep) {
		return start;
	}
	if (this->poweredTime == 0) {
		//TRACELNF("PwrConsumerProcess:request pwr")
		this->poweredTime = PowerloadManagement::get()->requestPin(this->keyPin);
		//TRACELN(this->poweredTime)
		// bit sleep - if got power - physical change state, if not - simple wait delay
		this->pause(10);
		return millis();
	}
	return IFirmwareProcess::run(start);
}

void PwrConsumerProcess::update(unsigned long ms) {
	// we've got POWER! ))
	switch (this->getWorkState())
	{
		case START: {
			TRACELNF("PwrConsumerProcess: start child processes");
			for (byte i = 0; i < this->taskCnt; i++) {
				this->getHost()->addProcess(this->taskIdList[i]);
				this->tasksArr[i] = ACTIVE;
			}
			return;
		}
		case DONE: {
			// shutdown
			//TRACELNF("PwrConsumerProcess: shut down child processes");
			TRACELNF("PwrConsumerProcess: shut down");
			this->clearState();
			this->releaseLoad();                        // required to unlock up pwr key
			this->deepSleep = true;
			this->getHost()->sendMessage(new ProcessOrderMessage(this->getId()));	// go to next of process list
			return;
		}
		default: {
			// no work should be done here?
			this->pause(15);
		}
	}
}

bool PwrConsumerProcess::handleMessage(IProcessMessage* msg) {
	if (msg->getType() == PRC_ORDER_MESSAGE)	{
		if (((ProcessOrderMessage*)msg)->getNextId() == this->getId()) {
			this->deepSleep = false;
			this->pause(CONSUMERPROCESSTIMEOUT);
			return true;
		}
		return false;
	}
	if (deepSleep || this->getWorkState() != ACTIVE) return false;
	return this->handleMessageLogic(msg);
}

void PwrConsumerProcess::releaseLoad() {
    if (this->poweredTime != 0) {
				if (!PowerloadManagement::get()->releasePin(this->poweredTime)) {
					TRACELNF("PwrConsumerProcess//got FALSE");
				}
				this->poweredTime = 0;
			}
}

PwrConsumerProcess::~PwrConsumerProcess() {
	// stop process
            this->releaseLoad();
	TRACELNF("PwrConsumerProcess::stop");
}
