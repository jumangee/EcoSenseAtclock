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

#include "meteo_cfg.h"
#include "processy_cfg.h"
#include "processy.h"

#include "meteo_main.h"
#include "meteo_mq136sensor.h"
#include "meteo_mq4sensor.h"
#include "rtcprocess.h"
#include "meteo_wifi_wifiesp_lite.h"
//#include "meteo_sensors_bme280.h"

#include "stuff.h"

//---[ IMPORTANT! ]-----------------
#define TOTAL_FACTORIES_INCLUDED 5
//----------------------------------

class MeteoClockFirmware: public IFirmware {
	MeteoClockFirmware();
	
	public:
		ProcessFactory getFactory(int pId);

		static IFirmware* get();

};

#endif
