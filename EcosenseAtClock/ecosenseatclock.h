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


	EcosenseAtClockFirmware();
	
	public:
		ProcessFactory getFactory(uint16_t pId);

		static IFirmware* get();

};

#endif
