/*
* processy Firmware Process Control Framework
*
*/

#ifndef _FIRMWARE_FRAMEWORK_MESSAGE_H
#define _FIRMWARE_FRAMEWORK_MESSAGE_H

class IFirmwareProcess;

#include "processy_process.h"

class IProcessMessage {
	public:
		IProcessMessage(IFirmwareProcess* from, int type) {
			this->sender = from;
			this->type = type;
		}

		String getSenderId();

		bool isSenderId(String & compareTo);

		IFirmwareProcess* getSender() {
			return sender;
		}

		bool isAnonymous() {
			return getSender() == NULL;
		}

		int getType() {
			return this->type;
		}

	private:
		IFirmwareProcess* sender;
		int type;
};

#endif
