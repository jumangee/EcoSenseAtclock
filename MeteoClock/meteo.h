/*
* MeteClock Firmware
*/


///////////////////////////////////////////////////
// PROCESSES NAMES

// MAX 14 CHARS!!!
#define PRC_MAIN "PRC_MAIN"
#define PRC_SENSORS "PRC_SENSORS"
//#define PRC_I2CSCANNER "PRC_I2CSCANNER"

// END: NAMES
//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_process.h"
#include "meteo_main.h"
#include "meteo_sensors.h"
//#include "meteo_i2c_scanner.h"

#include "stuff.h"

#define FACTORY(name, className) ProcessFactoryReg(name, &className::factory)
//#define FACTORY(name, className) ProcessFactoryReg(String(F(name)), [](String & name, IProcessMessage* msg){return className(id, msg);})


class MeteoClockFirmware: public IFirmware {

	enum PROCESS {
		MAIN		= (1),
		SENSORS		= (2)
	};

	MeteoClockFirmware();

	public:

		ProcessFactory getFactory(String & name);

		static IFirmware* get();

};

#endif
