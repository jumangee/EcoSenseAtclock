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
	EcosenseAtClockFirmware();
	
	public:
		ProcessFactory getFactory(int pId);

		static IFirmware* get();

};

#endif
