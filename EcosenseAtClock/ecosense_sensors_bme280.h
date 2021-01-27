/**
 * BME280 environment sensor process
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_SENSORS_H
#define _ECOSENSEATCLOCK_SENSORS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"

#include "forcedClimate/forcedClimate.h"

//class Adafruit_BME280;

class EnvironmentSensorsProcess: public IFirmwareProcess {
	private:
		//Adafruit_BME280* bme;
		ForcedClimate climateSensor;
		bool ready;
		bool initDone;

	public:
		EnvironmentSensorsProcess(uint16_t pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

		~EnvironmentSensorsProcess();

		void update(unsigned long ms);

		IProcessMessage* readBME280();
};

#endif
