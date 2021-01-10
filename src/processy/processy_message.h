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
		IProcessMessage(IFirmwareProcess* from, String type) {
			this->sender = from;
			this->type = type;
		}

		//@implement
		String getSenderId() {
			return sender->getId();
		}

		//@implement
		bool isSenderId(String compareTo) {
			return sender->isId(compareTo);
		}

		IFirmwareProcess* getSender() {
			return sender;
		}

		bool isAnonymous() {
			return getSender() == NULL;
		}

		String getType() {
			return this->type;
		}

	private:
		IFirmwareProcess* sender;
		String type;
};

#endif
