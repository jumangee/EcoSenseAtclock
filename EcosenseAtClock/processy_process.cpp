#include "processy_process.h"
#include "processy_cfg.h"
#include "stuff.h"
#include <Arduino.h>
#include "processy.h"

IFirmwareProcess::IFirmwareProcess(IProcessMessage* msg) {
	//this->processId = pId;
	this->lastUpdate = millis();
	#ifdef DEBUG_PRO_MS
	this->resetUsedMs();
	#endif
	this->pausedUpTo = 0;
	this->state = ProcessState::ACTIVE;
	this->handleMessage(msg);
}

void IFirmwareProcess::stop() {
	this->state = ProcessState::STOP;
}

bool IFirmwareProcess::isPaused(unsigned long start) {
	if (getState() == ProcessState::PAUSE) {
		if (start < this->pausedUpTo) {
			return true;
		}
		this->unPause();
	}
	return false;
}

unsigned long IFirmwareProcess::run(unsigned long start) {
	unsigned long ms = start - this->lastUpdate;
	this->update(ms);
	unsigned long endTime = millis();
	#ifdef DEBUG_PRO_MS
	this->usedMs += endTime - start;
	#endif
	this->lastUpdate = endTime;
	return endTime;
}

void IFirmwareProcess::pause(unsigned long upTo = 0) {
	if (this->state == ProcessState::STOP) return;
	this->state = ProcessState::PAUSE;
	this->pausedUpTo = millis() + upTo;
}

void IFirmwareProcess::unPause() {
	if (this->state == ProcessState::STOP) return;
	this->state = ProcessState::ACTIVE;
	this->pausedUpTo = 0;
}

bool IFirmwareProcess::handleMessage(IProcessMessage* msg) {
	return false;
};

void IFirmwareProcess::sendMessage(IProcessMessage* msg) {
	if (msg != NULL) {
		this->getHost()->sendMessage(msg);
	}
};

IFirmware* IFirmwareProcess::getHost() {
	return IFirmware::get();
}
