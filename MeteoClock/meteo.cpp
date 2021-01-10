#include "meteo.h"
#include <Arduino.h>
#include "processy_cfg.h"
#include "meteo_main.h"

MeteoClockFirmware::MeteoClockFirmware() : IFirmware(){
	#ifdef DEBUG_SERIAL
	Serial.println("START");
	#endif
};

void MeteoClockFirmware::init() {
	FACTORY_DEFAULT(PRC_MAIN, MainProcess)
	FACTORY(PRC_SENSORS, EnvironmentSensorsProcess)
	
	this->addProcess(PRC_MAIN);
	this->addProcess(PRC_SENSORS);
}

void MeteoClockFirmware::log(String msg) {
	#ifdef DEBUG_SERIAL
	Serial.println(msg);
	#endif
}

static IFirmware* MeteoClockFirmware::get() {
	if (IFirmware::instance == NULL) {
		Serial.begin(9600);
		delay(1000);
        		Serial.println("new MeteoClockFirmware");
		MeteoClockFirmware* meteo = new MeteoClockFirmware();
		meteo->init();
		IFirmware::instance = meteo;
        		return meteo;
	}
	return IFirmware::instance;
}
