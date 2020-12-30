/*
* MeteClock Firmware
*/
#define PRC_MAIN "PRC_MAIN"
#define PRC_SENSORS "PRC_SENSORS"
 
#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "ffpro.h"
#include "meteo_process.h"
#include "meteo_main.h"
#include "meteo_sensors.h"

class MeteoClockFirmware: public IFirmware {
	MeteoClockFirmware(): IFirmware() {

		this->registerFactory(PRC_MAIN, [](String id, IProcessMessage* msg) {
			return new MainProcess(id, msg);
		}, true);
		
		this->registerFactory(PRC_SENSORS, [](String id, IProcessMessage* msg) {
			return new SensorsProcess(id, msg);
		});
		
		this->addProcess(PRC_MAIN);
		this->addProcess(PRC_SENSORS);
		
		this->pauseProcess(PRC_SENSORS, 1000);
			
		#if (DEBUG_SERIAL == 1)
		Serial.begin(9600);
		#endif
	}
	
	public:
	
		void log(String msg) {
			#if (DEBUG_SERIAL == 1)
			Serial.println(msg);
			#endif
		}
		
		static IFirmware* get() {
			IFirmware* f = IFirmware::instance;
			if (f == NULL) {
				IFirmware::instance = new MeteoClockFirmware();
			}
			return f;
		}
};

#endif
