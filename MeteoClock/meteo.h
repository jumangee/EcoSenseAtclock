/*
* MeteClock Firmware
*/


///////////////////////////////////////////////////
// PROCESSES NAMES
#define PRC_MAIN "PRC_MAIN"
#define PRC_SENSORS "PRC_SENSORS"
#define PRC_I2CSCANNER "PRC_I2CSCANNER"
// END: NAMES
//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_process.h"
#include "meteo_main.h"
#include "meteo_sensors.h"

#define FACTORY_DEFAULT(name, className) this->registerFactory(name, [](String id, IProcessMessage* msg){return new className(id, msg);}, true);
#define FACTORY(name, className) this->registerFactory(name, [](String id, IProcessMessage* msg){return new className(id, msg);});

class MeteoClockFirmware: public IFirmware {

	MeteoClockFirmware();

	public:

		void init();

		void log(String msg);

		static IFirmware* get();

};

#endif
