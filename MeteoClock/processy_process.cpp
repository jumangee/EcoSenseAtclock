#include "processy_process.h"
#include "processy_cfg.h"
#include "stuff.h"
#include <Arduino.h>
#include "processy.h"

IFirmwareProcess::IFirmwareProcess(String id, IProcessMessage* msg) {
	this->processId = id;
	this->lastUpdate = millis();
	#ifdef DEBUG_PRO_MS
	this->resetUsedMs();
	#endif
	//TRACE(S("IFirmwareProcess::",this->processId.c_str(),"/", (this->pausedUpTo == NULL ? "NULL" : (String(*this->pausedUpTo)).c_str()) ))
	this->unPause();
}

IFirmwareProcess::~IFirmwareProcess() {
	this->unPause();
}

void IFirmwareProcess::log(String msg) {
	this->getHost()->log(msg);
}

bool IFirmwareProcess::isId(String compareTo) {
	return this->processId == compareTo;
}

unsigned long IFirmwareProcess::run(unsigned long start) {
	unsigned long ms = start - this->lastUpdate;
	//TRACE(S("IFirmwareProcess::run/",this->processId.c_str(),"/", (this->pausedUpTo == NULL ? "NULL" : (String(*this->pausedUpTo)).c_str()) ))
	if (this->pausedUpTo != NULL && start < this->pausedUpTo) {
		return start;	// no time wasting
	} else if (this->pausedUpTo) {
		this->unPause();
	}
	this->update(ms);
	unsigned long endTime = millis();
	#ifdef DEBUG_PRO_MS
	this->usedMs += endTime - start;
	#endif
	this->lastUpdate = endTime;
	return endTime;
}

void IFirmwareProcess::pause(unsigned long upTo = 0) {
	this->pausedUpTo = new unsigned long(millis() + upTo);
}

void IFirmwareProcess::unPause() {
	if (this->pausedUpTo != NULL) {
		delete this->pausedUpTo;
		this->pausedUpTo = NULL;
	}
}

bool IFirmwareProcess::handleMessage(IProcessMessage* msg) {
	return false;	// override this
}

IFirmware* IFirmwareProcess::getHost() {
	return IFirmware::get();
}
