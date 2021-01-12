/*
* MeteClock Firmware
*/

#ifndef _METEOCLOCK_SENSORS_H
#define _METEOCLOCK_SENSORS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"

//#include <Arduino.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

class Adafruit_BME280;

class EnvironmentSensorsProcess: public IFirmwareProcess {
	private:
		Adafruit_BME280* bme;
		bool ready;
		bool initDone;

	public:
		EnvironmentSensorsProcess(String id, IProcessMessage* msg);

		static IFirmwareProcess* factory(String & name, IProcessMessage* msg);

		~EnvironmentSensorsProcess();

		void init();

		void update(unsigned long ms);

		IProcessMessage* readBME280();
};

#endif
