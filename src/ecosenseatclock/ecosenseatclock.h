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

#ifndef _ECOSENSEATCLOCK_H
#define _ECOSENSEATCLOCK_H

#include "ecosense_cfg.h"
#include "processy_cfg.h"
#include "processy.h"

// ---[ PROCESSES ]---
#include "ecosense_main.h"
#include "ecosense_mq136sensor.h"
#include "ecosense_mq4sensor.h"
#include "rtcprocess.h"
#include "ecosense_wifi_wifiesp_lite.h"
//#include "ecosense_sensors_bme280.h"
// -------------------

#include "stuff.h"

class EcosenseAtClockFirmware: public IFirmware {
	//@implement
	//@include <Arduino.h>
	EcosenseAtClockFirmware(): IFirmware() {
		#ifdef PROCESSY_DEBUG_SERIAL
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
		//@include "ecosense_main.h"
		ProcessFactory getFactory(int pId) {
			const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
				FACTORY(PRC_MAIN, MainProcess)
				,FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess)
				,FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess)
				,FACTORY(PRC_RTC, RTClockProcess)
				,FACTORY(PRC_WIFI, WifiProcess)
				//,FACTORY(PRC_BME280SENSOR, EnvironmentSensorsProcess),
			};

			int len = *(&factoryList + 1) - factoryList;	//TOTAL_FACTORIES_INCLUDED

			for (byte i = 0; i < len; i++) {
				if (factoryList[i].id == pId) {
					return factoryList[i].factory;
				}
			}
			return NULL;
		}

		//@implement
		static IFirmware* get() {
			if (IFirmware::instance == NULL) {
        		TRACELNF("new EcosenseAtClockFirmware");
				IFirmware::instance = new EcosenseAtClockFirmware();
			}
			return IFirmware::instance;
		}

};

#endif
