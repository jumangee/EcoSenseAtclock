/*
* MeteClock Firmware
*/
 
#ifndef _METEOCLOCK_MAIN_H
#define _METEOCLOCK_MAIN_H

#include "ffpro.h"
#include "ffpro_process.h"

#include "meteo.h"
#include "meteo_process.h"
#include "meteo_messages.h"

#include <Arduino.h>

class MainProcess: public IMeteoClockProcess {
  public:
  	MainProcess(String id, IProcessMessage* msg): IMeteoClockProcess(id, msg) {
  		this->log("MainProcess::start");
  	}
  	
  	~MainProcess() {
  		// stop process
  		this->log("MainProcess::stop");
  	}
  	
  	void update(unsigned long ms) {
		this->log("MainProcess::run ");
		if (random(1, 10) == 5) {
			this->getHost()->pauseProcess(PRC_MAIN, 2000);
		}
  	}
	
	bool handleMessage(IProcessMessage* msg) {
		if (msg->getType() == MSG_COMMAND_ALL) {
			CmdMessage* cmd = (CmdMessage*)msg;
			if (cmd->getCmd() == CmdMessage::Cmd::UNPAUSE_ALL) {
				this->unPause();
				return false; // continue handling by others
			}
			if (cmd->getCmd() == CmdMessage::Cmd::PAUSE_ALL) {
				this->pause();
				return false; // continue handling by others
			}
		}
		return false;
	}
};

#endif
