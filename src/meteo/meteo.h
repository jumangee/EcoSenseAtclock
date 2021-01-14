/*
* MeteClock Firmware
*/


///////////////////////////////////////////////////
// PROCESSES NAMES

// MAX 14 CHARS!!!
#define PRC_MAIN		101
#define PRC_SENSORS		102
#define PRC_I2CSCANNER	103
#define PRC_MQ136SENSOR	104
#define PRC_MQ4SENSOR	105
#define PRC_RTC			106
#define PRC_WIFI		107


// END: NAMES
//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_process.h"
#include "meteo_main.h"
#include "meteo_mq136sensor.h"
#include "meteo_mq4sensor.h"
#include "rtcprocess.h"

#include "stuff.h"

#define FACTORY(name, className) ProcessFactoryReg(name, &className::factory)
//#define FACTORY(name, className) ProcessFactoryReg(String(F(name)), [](String & name, IProcessMessage* msg){return className(id, msg);})


class MeteoClockFirmware: public IFirmware {
	//@implement
	//@include <Arduino.h>
	MeteoClockFirmware(): IFirmware() {
		#ifdef DEBUG_SERIAL
		TRACELNF("START");
		#endif

		this->addProcess(PRC_MAIN);

		this->addProcess(PRC_MQ136SENSOR);
		this->addProcess(PRC_MQ4SENSOR);
		this->addProcess(PRC_MQ4SENSOR);
		this->addProcess(PRC_RTC);
		this->addProcess(PRC_WIFI);
	};
	
	public:
		//@implement
		//@include "meteo_main.h"
		ProcessFactory getFactory(int pId) {
			const static byte size PROGMEM = {5};					//	factories count!
			const static ProcessFactoryReg factoryList[size] = {	//	factories list
				FACTORY(PRC_MAIN, MainProcess),
				FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess),
				FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess),
				FACTORY(PRC_RTC, RTClockProcess),
				FACTORY(PRC_WIFI, WifiProcess)
			};

			for (byte i = 0; i < size; i++) {
				if (factoryList[i].id == pId) {
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
			}
			return IFirmware::instance;
		}

};

#endif
