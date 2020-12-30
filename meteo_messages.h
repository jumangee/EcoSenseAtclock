#define MSG_COMMAND_ALL "COMMAND_MESSAGE"
#define MSG_TEXT_ALL "TEXT_MESSAGE"

#ifndef _METEO_MESSAGES_H
#define _METEO_MESSAGES_H

class IFirmwareProcess;

#include "ffpro_message.h"

class CmdMessage: public IProcessMessage {
	public:
		enum Cmd {
			UNPAUSE_ALL,
			PAUSE_ALL
		};
		
		CmdMessage(IFirmwareProcess* from, CmdMessage::Cmd cmd): IProcessMessage(from, MSG_COMMAND_ALL) {
			this->cmd = cmd;
		}
		
		CmdMessage(CmdMessage::Cmd cmd): IProcessMessage(NULL, MSG_COMMAND_ALL) {
			this->cmd = cmd;
		}
		
		Cmd getCmd() {
			return this->cmd;
		}

	private:
		Cmd cmd;
};

#endif
