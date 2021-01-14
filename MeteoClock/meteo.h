/*
* MeteClock Firmware
*/


///////////////////////////////////////////////////
// PROCESSES NAMES

// MAX 14 CHARS!!!
#define PRC_MAIN		101
#define PRC_SENSORS		102
#define PRC_I2CSCANNER	103
#define PRC_MQ136SENSOR	104
#define PRC_MQ4SENSOR	105
#define PRC_RTC			106


// END: NAMES
//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_process.h"
#include "meteo_main.h"
#include "meteo_mq136sensor.h"
#include "meteo_mq4sensor.h"
#include "rtcprocess.h"

#include "stuff.h"

#define FACTORY(name, className) ProcessFactoryReg(name, &className::factory)
//#define FACTORY(name, className) ProcessFactoryReg(String(F(name)), [](String & name, IProcessMessage* msg){return className(id, msg);})


class MeteoClockFirmware: public IFirmware {
	MeteoClockFirmware();
	
	public:
		ProcessFactory getFactory(int pId);

		static IFirmware* get();

};

#endif
