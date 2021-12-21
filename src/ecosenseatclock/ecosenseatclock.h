/**
 * EcoSense@Clock
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_H
#define _ECOSENSEATCLOCK_H

#include "ecosense_cfg.h"
#include "processy_cfg.h"
#include "processy.h"

#include "pwrload_mngmnt.h"
#include "adcmux_mngmnt.h"

// ---[ PROCESSES ]---
#include "display_process.h"
#include "rtc_process.h"
#include "wifi_process.h"
#include "bme280_process.h"

#include "mq136sensor_process.h"
#include "mq4sensor_process.h"
#include "cjmcu1100sensor_process.h"
#include "mhz19sensor_process.h"
#include "mq135sensor_process.h"
#include "mq7sensor_process.h"

#include "pwrconsumer_process.h"
#include "pwrconsumer1process.h"
#include "pwrconsumer2process.h"
#include "pwrconsumer3process.h"
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

		addProcess(PRC_DISPLAY);
		addProcess(PRC_RTC);
		//addProcess(PRC_WIFI);
		addProcess(PRC_BME280);

		PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));	//EcosenseAtClockFirmware::PwrMngmtPins, (*(&PwrMngmtPins + 1) - PwrMngmtPins)
		TRACELNF("Power management DONE");

		ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);
		TRACELNF("AdcMux management DONE");
		
		addProcess(PRC_CONSUMER1, ProcessOrderMessage::start());
	};
	
	public:
		//@implement
		//!@include "ecosense_main.h"
		ProcessFactory getFactory(uint16_t pId) {
			const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
				FACTORY(DisplayProcess)
				,FACTORY(RTClockProcess)
				//,FACTORY(WifiProcess)
				,FACTORY(BME280SensorProcess)

				,FACTORY(PwrConsumer1Process)
				,FACTORY(PwrConsumer2Process)
				,FACTORY(PwrConsumer3Process)

				,FACTORY(MQ136SensorProcess)
				//,FACTORY(MQ7SensorProcess)
				,FACTORY(MQ135SensorProcess)
				,FACTORY(MQ4SensorProcess)
				//,FACTORY(PPD42SensorProcess)
				//,FACTORY(ZE08CH2OSensorProcess)
				,FACTORY(CJMCU1100SensorProcess)
				,FACTORY(MHZ19SensorProcess)
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
				IFirmware::instance = new EcosenseAtClockFirmware();
			}
			return IFirmware::instance;
		}

};

#endif
