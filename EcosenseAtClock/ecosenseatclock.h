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
	const static byte PwrMngmtPins[];
	const static byte AdcMuxMngmtPins[];


	EcosenseAtClockFirmware();
	
	public:
		static IFirmware* get();

	protected:
		virtual void handlerProcessDebugTimer(unsigned long dT);

};

#endif
