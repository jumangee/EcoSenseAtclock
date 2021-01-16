/**
 * Application class (METEOCLOCK)
 * for Processy Firmware Framework
 */

///////////////////////////////////////////////////
// PROCESS IDs

#define PRC_MAIN			101
#define PRC_BME280SENSOR	102
#define PRC_I2CSCANNER		103
#define PRC_MQ136SENSOR		104
#define PRC_MQ4SENSOR		105
#define PRC_RTC				106
#define PRC_WIFI			107

//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_main.h"
#include "meteo_mq136sensor.h"
#include "meteo_mq4sensor.h"
#include "rtcprocess.h"
#include "meteo_wifi_wifiesp_lite.h"
#include "meteo_sensors_bme280.h"

#include "stuff.h"

//---[ IMPORTANT! ]-----------------
#define TOTAL_FACTORIES_INCLUDED 6
//----------------------------------

class MeteoClockFirmware: public IFirmware {
	//@implement
	//@include <Arduino.h>
	MeteoClockFirmware(): IFirmware() {
		#ifdef DEBUG_SERIAL
		TRACELNF("START");
		#endif

		addProcess(PRC_MAIN);
		addProcess(PRC_MQ136SENSOR);
		addProcess(PRC_MQ4SENSOR);
		addProcess(PRC_RTC);
		addProcess(PRC_WIFI);
		addProcess(PRC_BME280SENSOR);
	};
	
	public:
		//@implement
		//@include "meteo_main.h"
		ProcessFactory getFactory(int pId) {
			const static ProcessFactoryReg factoryList[TOTAL_FACTORIES_INCLUDED] = {	//	factories list
				FACTORY(PRC_MAIN, MainProcess),
				FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess),
				FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess),
				FACTORY(PRC_RTC, RTClockProcess),
				FACTORY(PRC_WIFI, WifiProcess),
				FACTORY(PRC_BME280SENSOR, EnvironmentSensorsProcess),
			}; //--[ size must be eq TOTAL_FACTORIES_INCLUDED ]--

			for (byte i = 0; i < TOTAL_FACTORIES_INCLUDED; i++) {
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
