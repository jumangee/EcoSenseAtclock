/**
 * BME280 environment sensor process
 * for Processy Firmware Framework
 */

#ifndef _METEOCLOCK_SENSORS_H
#define _METEOCLOCK_SENSORS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"

#define CO2_SENSOR

#ifdef CO2_SENSOR
	#include "MHZ19_uart/MHZ19_uart.h"
	#define MHZ19_RXPIN 3
	#define MHZ19_TXPIN 4
#endif

class Adafruit_BME280;

class EnvironmentSensorsProcess: public IFirmwareProcess {
	private:
		Adafruit_BME280* bme;
		#ifdef CO2_SENSOR
		MHZ19_uart		mhz19;
		bool			mhz19active;
		#endif
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
