/*
* processy Firmware Process Control Framework
*
*/

#ifndef _FIRMWARE_FRAMEWORK_MESSAGE_H
#define _FIRMWARE_FRAMEWORK_MESSAGE_H

class IFirmwareProcess;

#include "processy_process.h"
#include <Arduino.h>
#include <math.h>

//--- MSG TYPES -------------------
#define FREEMEM_MESSAGE 1000
//---------------------------------

class IProcessMessage {
	public:
		IProcessMessage(IFirmwareProcess* from, uint16_t type) {
			this->sender = from;
			this->type = type;
		}

		IFirmwareProcess* getSender() {
			return sender;
		}

		uint16_t getType() {
			return this->type;
		}

	private:
		IFirmwareProcess* sender;
		uint16_t type;
};

class MemUsageMessage: public IProcessMessage {
	public:
		MemUsageMessage(int free): IProcessMessage(NULL, FREEMEM_MESSAGE) {
			this->free = byte(round(((float)free * 100.0)/2048));
		}

		byte getFreemem() {
			return this->free;
		}

	private:
		byte	free;
};

#endif
