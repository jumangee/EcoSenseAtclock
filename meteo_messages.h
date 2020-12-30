#ifndef _METEO_MESSAGES_H
#define _METEO_MESSAGES_H

class IFirmwareProcess;

#include "ffpro_message.h"

class TextMessage: public IProcessMessage {
	public:
		TextMessage(IFirmwareProcess* from, String text): IProcessMessage(from, "TEXT_MESSAGE") {
			this->text = text;
		}
		
		TextMessage(String text): IProcessMessage(NULL, "TEXT_MESSAGE") {
			this->text = text;
		}
		
		String getText() {
			return this->text;
		}

	private:
		String text;
};

#endif
