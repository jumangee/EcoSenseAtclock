#include "processy_process.h"
#include "processy_cfg.h"
#include "stuff.h"
#include <Arduino.h>
#include "processy.h"

IFirmwareProcess::IFirmwareProcess(int pId, IProcessMessage* msg) {
	this->processId = pId;
	this->lastUpdate = millis();
	#ifdef DEBUG_PRO_MS
	this->resetUsedMs();
	#endif
	this->pausedUpTo = 0;
	this->state = ProcessState::ACTIVE;
	this->handleMessage(msg);
}

IFirmwareProcess::~IFirmwareProcess() {
}

bool IFirmwareProcess::isId(int compareTo) {
	return this->processId == compareTo;
}

void IFirmwareProcess::stop() {
	this->state = ProcessState::STOP;
}

unsigned long IFirmwareProcess::run(unsigned long start) {
	//TRACE(S("IFirmwareProcess//start=", String(start).c_str(), ", lastUpdate=", String(this->lastUpdate).c_str() ))
	//TRACE(S("IFirmwareProcess::run/",this->processId.c_str(),"/start=", String(start).c_str(),", pause=", String(this->pausedUpTo).c_str()) )
	if (this->state == ProcessState::PAUSE) {
		if (start < this->pausedUpTo) {
			return start;	// no time wasting
		}
		this->unPause();
	}
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
	this->state = ProcessState::PAUSE;
	this->pausedUpTo = millis() + upTo;
}

void IFirmwareProcess::unPause() {
	this->state = ProcessState::ACTIVE;
	this->pausedUpTo = 0;
}

bool IFirmwareProcess::handleMessage(IProcessMessage* msg) {
	return false;
};

IFirmware* IFirmwareProcess::getHost() {
	return IFirmware::get();
}
