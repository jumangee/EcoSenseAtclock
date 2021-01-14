#include "meteo.h"
#include <Arduino.h>
#include "meteo_main.h"

MeteoClockFirmware::MeteoClockFirmware() : IFirmware(){
	#ifdef DEBUG_SERIAL
	TRACELNF("START");
	#endif
	this->addProcess(PRC_MAIN);
	this->addProcess(PRC_MQ136SENSOR);
	this->addProcess(PRC_MQ4SENSOR);
	this->addProcess(PRC_MQ4SENSOR);
	this->addProcess(PRC_RTC);
};

ProcessFactory MeteoClockFirmware::getFactory(int pId) {
	const static byte size PROGMEM = {4};
	const static ProcessFactoryReg factoryList[size] = {
		FACTORY(PRC_MAIN, MainProcess),
		FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess),
		FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess),
		FACTORY(PRC_RTC, RTClockProcess)
	};
	for (byte i = 0; i < size; i++) {
		if (factoryList[i].id == pId) {
			return factoryList[i].factory;
		}
	}
	return NULL;
}

static IFirmware* MeteoClockFirmware::get() {
	if (IFirmware::instance == NULL) {
        		TRACELNF("new MeteoClockFirmware");
		IFirmware::instance = new MeteoClockFirmware();
	}
	return IFirmware::instance;
}
