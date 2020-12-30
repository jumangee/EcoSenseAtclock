/*
* MeteClock Firmware
*/
 
#ifndef _METEOCLOCK_SENSORS_H
#define _METEOCLOCK_SENSORS_H

#include "ffpro.h"
#include "ffpro_process.h"

#include "meteo.h"

#include <Arduino.h>

class SensorsProcess: public IMeteoClockProcess {
  public:
  	SensorsProcess(String id, IProcessMessage* msg): IMeteoClockProcess(id, msg) {
  		this->log("SensorsProcess::start");
  	}
  	
  	~SensorsProcess() {
  		// stop process
  		this->log("SensorsProcess::stop");
  	}
  	
  	void update(unsigned long ms) {
		this->log("SensorsProcess::run");
		this->readSensors();
		if (this->sensor1Value == 50) {
			this->getHost()->sendMessage(new CmdMessage(CmdMessage::Cmd::UNPAUSE_ALL));
		}
  	}
  	
  	void readSensors() {
  		// читаем показания датчиков
  		sensor1Value = random(10, 100);
  		sensor2Value = random(1000, 10000);
  	}
	
	private:
		int sensor1Value;
		int sensor2Value;
};

#endif
