/**
 * BME280 environment sensor process
 * for Processy Firmware Framework
 */

#ifndef _METEOCLOCK_SENSORS_H
#define _METEOCLOCK_SENSORS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"

class Adafruit_BME280;

class EnvironmentSensorsProcess: public IFirmwareProcess {
	private:
		Adafruit_BME280* bme;
		bool ready;
		bool initDone;

	public:
		EnvironmentSensorsProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		~EnvironmentSensorsProcess();

		void update(unsigned long ms);

		IProcessMessage* readBME280();
};

#endif
