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
	if (this->findTask(prcId) == -1) {
		TaskInfo* task = new TaskInfo();
		task->prcId = prcId;
		task->state = NONE;
		tasks.add(task);
	}
}

int PwrConsumerProcess::findTask(uint16_t id) {
	for (byte i = 0; i < this->tasks.size(); i++) {
		if (this->tasks.get(i)->prcId == id) {
			return i;
		}
	}
	return -1;
}

void PwrConsumerProcess::taskDone(uint16_t process_id) {
	int pos = this->findTask(process_id);
	if (pos == -1) return;
	this->tasks.get(pos)->state = DONE;
	this->getHost()->stopProcess(process_id);
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

bool PwrConsumerProcess::handleMessage(IProcessMessage* msg) {
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
	for (int i = this->tasks.size()-1; i >= 0; i--) {
		delete this->tasks.remove(i);
	}
	TRACELNF("PwrConsumerProcess::stop");
}
