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
#include "wifi_process.h"
#include "bme280_process.h"

#include "btn_process.h"

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


	EcosenseAtClockFirmware();
	
	public:
		ProcessFactory getFactory(uint16_t pId);

		static IFirmware* get();

};

#endif
