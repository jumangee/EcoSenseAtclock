/*
* MeteClock Firmware
*/


///////////////////////////////////////////////////
// PROCESSES NAMES

// MAX 14 CHARS!!!
#define PRC_MAIN "PRC_MAIN"
#define PRC_SENSORS "PRC_SENSORS"
//#define PRC_I2CSCANNER "PRC_I2CSCANNER"

// END: NAMES
//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_process.h"
#include "meteo_main.h"
#include "meteo_sensors.h"
//#include "meteo_i2c_scanner.h"

#include "stuff.h"

#define FACTORY(name, className) ProcessFactoryReg(name, &className::factory)
//#define FACTORY(name, className) ProcessFactoryReg(String(F(name)), [](String & name, IProcessMessage* msg){return className(id, msg);})


class MeteoClockFirmware: public IFirmware {

	enum PROCESS {
		MAIN		= (1),
		SENSORS		= (2)
	};

	//@implement
	//@include <Arduino.h>
	MeteoClockFirmware(): IFirmware() {
		#ifdef DEBUG_SERIAL
		TRACELNF("START");
		#endif

		{
			String s = SF(PRC_MAIN);
			this->addProcess(s);
		}
		{
			String s = SF(PRC_SENSORS);
			this->addProcess(s);
		}
	};

	public:

		//@implement
		//@include "meteo_main.h"
		ProcessFactory getFactory(String & name) {
			String PRC_MAINs = SF(PRC_MAIN);
			String PRC_SENSORSs = SF(PRC_SENSORS);

			const static byte size PROGMEM = {2};
			const static ProcessFactoryReg factoryList[size] = {
				FACTORY(PRC_MAINs, MainProcess),
				FACTORY(PRC_SENSORSs, EnvironmentSensorsProcess)
			};

			for (byte i = 0; i < size; i++) {
				if (factoryList[i].id.equals(name)) {
					return factoryList[i].factory;
				}
			}
			return NULL;
		}

		//@implement
		static IFirmware* get() {
			if (IFirmware::instance == NULL) {
        		TRACELNF("new MeteoClockFirmware");
				IFirmware::instance = new MeteoClockFirmware();
				//((MeteoClockFirmware*)IFirmware::instance)->init();
			}
			return IFirmware::instance;
		}

};

#endif
