/**
 * Application class (METEOCLOCK)
 * for Processy Firmware Framework
 */

///////////////////////////////////////////////////
// PROCESS IDs

#define PRC_MAIN			101
#define PRC_BME280SENSOR	102
#define PRC_I2CSCANNER		103
#define PRC_RTC				106
#define PRC_WIFI			107
#define PRC_CONSUMER1		221
#define PRC_CONSUMER2		222
#define PRC_CONSUMER3		223
#define PRC_MONITOR			224

#define PRC_DUMB1			901
#define PRC_DUMB2			902
#define PRC_DUMB3			903
#define PRC_DUMB4			904

#define PRC_PPD42SENSOR		301
#define PRC_MQ135SENSOR		302
#define PRC_MQ4SENSOR		303
#define PRC_MQ136SENSOR		304
#define PRC_MQ4SENSOR		305
#define PRC_ZE08SENSOR		306
#define PRC_MQ7SENSOR		307
#define PRC_CJMCU1100SENSOR	308
#define PRC_MHZ19SENSOR		309

//////////////////////////////////////////////////

#ifndef _ECOSENSEATCLOCK_H
#define _ECOSENSEATCLOCK_H

#include "ecosense_cfg.h"
#include "processy_cfg.h"
#include "processy.h"

#include "pwrload_mngmnt.h"
#include "adcmux_mngmnt.h"

// ---[ PROCESSES ]---
#include "ecosense_main.h"
#include "rtcprocess.h"
#include "ecosense_wifi_wifiesp_lite.h"
#include "ecosense_sensors_bme280.h"

#include "ecosense_mq136sensor.h"
#include "ecosense_mq4sensor.h"
#include "ecosense_ZE08CH20sensor.h"
#include "ecosense_cjmcu1100sensor.h"
#include "ecosense_mhz19sensor.h"
#include "ecosense_mq135sensor.h"
#include "ecosense_PPD42sensor.h"
#include "ecosense_mq7sensor.h"

#include "pwrconsumer_process.h"
#include "pwrconsumer1process.h"
#include "pwrconsumer2process.h"
#include "pwrconsumer3process.h"

//#include "dumb1_process.h"
//#include "dumb2_process.h"
// -------------------

#include "stuff.h"

class EcosenseAtClockFirmware: public IFirmware {
	const static byte PwrMngmtPins[];
	const static byte AdcMuxMngmtPins[];

	//@cpp
const static byte EcosenseAtClockFirmware::PwrMngmtPins[] = PWRMNGMTPINS;
	//@cpp
const static byte EcosenseAtClockFirmware::AdcMuxMngmtPins[] = ADCMUXPINS;

	//@implement
	//@include <Arduino.h>
	EcosenseAtClockFirmware(): IFirmware() {
		#ifdef PROCESSY_DEBUG_SERIAL
		Serial.begin(9600);
		delay(1000);		
		TRACELNF("START");
		#endif

		analogReference(EXTERNAL);	// important!

		addProcess(PRC_MAIN);
		//addProcess(PRC_MQ136SENSOR);
		//addProcess(PRC_MQ4SENSOR);
		addProcess(PRC_RTC);
		addProcess(PRC_WIFI);
		addProcess(PRC_BME280SENSOR);

		PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));	//EcosenseAtClockFirmware::PwrMngmtPins, (*(&PwrMngmtPins + 1) - PwrMngmtPins)
		TRACELNF("Power management DONE");

		ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);
		TRACELNF("AdcMux management DONE");
		
		addProcess(PRC_CONSUMER1);
		addProcess(PRC_CONSUMER2);
		addProcess(PRC_CONSUMER3);

		this->sendMessage(ProcessOrderMessage::start());
	};
	
	public:
		//@implement
		//!@include "ecosense_main.h"
		ProcessFactory getFactory(uint16_t pId) {
			const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
				FACTORY(PRC_MAIN, MainProcess)
				,FACTORY(PRC_RTC, RTClockProcess)
				,FACTORY(PRC_WIFI, WifiProcess)
				,FACTORY(PRC_BME280SENSOR, EnvironmentSensorsProcess),

				FACTORY(PRC_CONSUMER1, PwrConsumer1Process)
				,FACTORY(PRC_CONSUMER2, PwrConsumer2Process)
				,FACTORY(PRC_CONSUMER3, PwrConsumer3Process)

				,FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess)
				,FACTORY(PRC_MQ7SENSOR, MQ7SensorProcess)
				,FACTORY(PRC_MQ135SENSOR, MQ135SensorProcess)
				,FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess)
				,FACTORY(PRC_PPD42SENSOR, PPD42SensorProcess)
				,FACTORY(PRC_ZE08SENSOR, ZE08CH2OSensorProcess)
				,FACTORY(PRC_CJMCU1100SENSOR, CJMCU1100SensorProcess)
				,FACTORY(PRC_MHZ19SENSOR, MHZ19SensorProcess)

				//,FACTORY(PRC_DUMB1, Dumb1Process)
				//,FACTORY(PRC_DUMB2, Dumb2Process)
				//,FACTORY(PRC_DUMB3, Dumb1Process)
				//,FACTORY(PRC_DUMB4, Dumb2Process)
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
