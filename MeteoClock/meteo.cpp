#include "meteo.h"
#include <Arduino.h>
#include "meteo_main.h"

MeteoClockFirmware::MeteoClockFirmware() : IFirmware(){
	#ifdef PROCESSY_DEBUG_SERIAL
	TRACELNF("START");
	#endif
	addProcess(PRC_MAIN);
	addProcess(PRC_MQ136SENSOR);
	addProcess(PRC_MQ4SENSOR);
	addProcess(PRC_RTC);
	addProcess(PRC_WIFI);
	addProcess(PRC_BME280SENSOR);
};

ProcessFactory MeteoClockFirmware::getFactory(int pId) {
	const static ProcessFactoryReg factoryList[TOTAL_FACTORIES_INCLUDED] = {	//	factories list
		FACTORY(PRC_MAIN, MainProcess)
		,FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess)
		,FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess)
		,FACTORY(PRC_RTC, RTClockProcess)
		,FACTORY(PRC_WIFI, WifiProcess)
		//,FACTORY(PRC_BME280SENSOR, EnvironmentSensorsProcess),
	}; //--[ size must be eq TOTAL_FACTORIES_INCLUDED ]--
	for (byte i = 0; i < TOTAL_FACTORIES_INCLUDED; i++) {
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
