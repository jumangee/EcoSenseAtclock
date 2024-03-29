/**
 * EcoSense@Clock
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_H
#define _ECOSENSEATCLOCK_H

#include "processy_cfg.h"
#include "ecosense_cfg.h"
#include "processy.h"

#include "pwrload_mngmnt.h"
#include "adcmux_mngmnt.h"

// ---[ PROCESSES ]---
#include "display_process.h"
#include "rtc_process.h"
#include "bme280_process.h"
#include "mainbtn_process.h"
#include "mhz19sensor_process.h"

#if NOWIFI_BUILD != 1
	#include "wifi_process.h"
#endif

#if SLIM_BUILD != 1
	#include "mq136sensor_process.h"
	#include "mq4sensor_process.h"
	#include "cjmcu1100sensor_process.h"
	#include "mq135sensor_process.h"
	#include "mq7sensor_process.h"
	#include "ppd42ns_process.h"

	#include "pwrconsumer_process.h"
	#include "pwrconsumer1process.h"
	#include "pwrconsumer2process.h"
	#include "pwrconsumer3process.h"
#endif

// -------------------

#include "stuff.h"

class EcosenseAtClockFirmware: public IFirmware {
	PowerloadManagement* pwrLoadMngmnt;

	const static byte PwrMngmtPins[];
	const static byte AdcMuxMngmtPins[];

	//@cpp
const static byte EcosenseAtClockFirmware::PwrMngmtPins[] = PWRMNGMTPINS;
	//@cpp
const static byte EcosenseAtClockFirmware::AdcMuxMngmtPins[] = ADCMUXPINS;

	//@implement
	//@include <Arduino.h>
	EcosenseAtClockFirmware(): IFirmware() {
		#if PROCESSY_DEBUG_SERIAL == 1
		Serial.begin(9600);
		delay(1000);		
		TRACELNF("START");
		#endif

		analogReference(EXTERNAL);	// important!

		// ---[ TASK REGISTRATION ]---

		PROCESS_REG(DisplayProcess);
		PROCESS_REG(RTClockProcess);
		#if NOWIFI_BUILD != 1
			PROCESS_REG(WifiProcess);
		#endif
		PROCESS_REG(BME280SensorProcess);
		PROCESS_REG(EcoSenseAtClockBtnProcess);
		PROCESS_REG(MHZ19SensorProcess);

		#if SLIM_BUILD != 1
			PROCESS_REG(PwrConsumer1Process);
			PROCESS_REG(PwrConsumer2Process);
			PROCESS_REG(PwrConsumer3Process);

			PROCESS_REG(MQ136SensorProcess);
			PROCESS_REG(MQ7SensorProcess);
			PROCESS_REG(MQ135SensorProcess);
			PROCESS_REG(MQ4SensorProcess);
			PROCESS_REG(PPD42SensorProcess);
			PROCESS_REG(CJMCU1100SensorProcess);
		#endif

		// ---[ STARTUP ]---

		addProcess(PRC_DISPLAY);
		addProcess(PRC_RTC);
		#if NOWIFI_BUILD != 1
			addProcess(PRC_WIFI);
		#endif
		addProcess(PRC_BME280);
		addProcess(PRC_BTN);
		addProcess(PRC_MHZ19);

		#if SLIM_BUILD != 1
			pwrLoadMngmnt = new PowerloadManagement(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins)); //PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));
			ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);
		
			addProcess(PRC_CONSUMER1);
		#endif
	};
	
	public:
		//@implement
		static IFirmware* get() {
			if (IFirmware::instance == NULL) {
				IFirmware::instance = new EcosenseAtClockFirmware();
			}
			return IFirmware::instance;
		}

		/**
		 * @brief Used by PowerloadManagement::get()
		 * 
		 * @return PowerloadManagement* 
		 */
		PowerloadManagement* getPwrLoadMngmnt() {
			return pwrLoadMngmnt;
		}

	protected:
		//@implement
		//@include "stuff.h"
		//@include "MemoryFree.h"
		virtual void handlerProcessDebugTimer(unsigned long dT) {
			#if DEBUG_PRO_MS == 2
				byte processCount = 0;
				for (uint16_t i = 0; i < this->processListSize; i++) {
					IFirmwareProcessRegistration* reg = this->processList[i];
					if (reg->isActive()) {
						processCount++;
					}
				}
				this->sendMessage(new SelfReportMessage(processCount, freeMemory()));
				TRACEF("SYS REPORT: processes=")
				TRACE(processCount)
				TRACEF(", freemem=")
				TRACELN(freeMemory())
			#else
				IFirmware::handlerProcessDebugTimer(dT);
			#endif
		}
};

#endif
