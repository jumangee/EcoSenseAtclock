/*
* MeteClock Firmware MAIN process
*/

#ifndef _METEOCLOCK_MAIN_H
#define _METEOCLOCK_MAIN_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "meteo_process.h"
#include "meteo_messages.h"

//#include <Arduino.h>

class MainProcess: public IFirmwareProcess {
	private:

	public:
		//@implement
		MainProcess(String id, IProcessMessage* msg): IFirmwareProcess(id, msg) {
			//this->log("MainProcess::start");
		}

		//@implement
		~MainProcess() {
			// stop process
			this->log("MainProcess::stop");
		}

		//@implement
		void update(unsigned long ms) {
			this->log("MainProcess::run");
		}

		//@implement
		//@include "stuff.h"
		bool handleMessage(IProcessMessage* msg) {
			if (msg->getType() == MSG_ENV_DATA) {
				EnvDataMessage* msg = (EnvDataMessage*)msg;
				if (msg->isActive()) {
					this->log(S("BME data: T=", String(msg->getTemp()).c_str(),
					"C, Hum=", String(msg->getHumidity()).c_str(), "%, Pres=", 
					String(msg->getPressure()).c_str(), "mm, Alt=",
					String(msg->getAltitude()).c_str(), "m"));
				} else {
					this->log("BME Sensors is passive!");
				}
			}
			return false;
		}
};

#endif
