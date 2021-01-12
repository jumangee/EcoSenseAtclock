#include "processy_process.h"
#include "processy_cfg.h"
#include "stuff.h"
#include <Arduino.h>
#include "processy.h"

IFirmwareProcess::IFirmwareProcess(String & id, IProcessMessage* msg) {
	this->processId = String(id);
	this->lastUpdate = millis();
	#ifdef DEBUG_PRO_MS
	this->resetUsedMs();
	#endif
	//TRACE(S("IFirmwareProcess::",this->processId.c_str(),"/", (this->pausedUpTo == NULL ? "NULL" : (String(*this->pausedUpTo)).c_str()) ))
	this->pausedUpTo = 0;
	this->handleMessage(msg);
}

IFirmwareProcess::~IFirmwareProcess() {
}

bool IFirmwareProcess::isId(String & compareTo) {
	return this->processId.equals(compareTo);
}

unsigned long IFirmwareProcess::run(unsigned long start) {
	//TRACE(S("IFirmwareProcess//start=", String(start).c_str(), ", lastUpdate=", String(this->lastUpdate).c_str() ))
	unsigned long ms = start - this->lastUpdate;
	//TRACE(S("IFirmwareProcess::run/",this->processId.c_str(),"/start=", String(start).c_str(),", pause=", String(this->pausedUpTo).c_str()) )
	if (this->pausedUpTo != 0) {
		if (start < this->pausedUpTo) {
			return start;	// no time wasting
		}
	}
	this->unPause();
	this->update(1);
	unsigned long endTime = millis();
	#ifdef DEBUG_PRO_MS
	this->usedMs += endTime - start;
	#endif
	this->lastUpdate = endTime;
	return endTime;
}

void IFirmwareProcess::pause(unsigned long upTo = 0) {
	this->pausedUpTo = millis() + upTo;
}

void IFirmwareProcess::unPause() {
	this->pausedUpTo = 0;
}

bool IFirmwareProcess::handleMessage(IProcessMessage* msg) {
	return false;
};

IFirmware* IFirmwareProcess::getHost() {
	return IFirmware::get();
}
