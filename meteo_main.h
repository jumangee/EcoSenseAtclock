/*
* MeteClock Firmware
*/
 
#ifndef _METEOCLOCK_MAIN_H
#define _METEOCLOCK_MAIN_H

#include "ffpro.h"
#include "ffpro_process.h"

#include "meteo_messages.h"

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
		if (msg->getType() == "TEXT_MESSAGE") {
			TextMessage* cmd = (TextMessage*)msg;
			if (cmd->getText() == "UNPAUSE_ALL") {
				this->unPause();
				return true; // message handling done
			}
			if (cmd->getText() == "STOP_ALL") {
				this->getHost()->stopAll();
				return true; // message handling done
			}
		}
		return false;
	}
};

#endif
