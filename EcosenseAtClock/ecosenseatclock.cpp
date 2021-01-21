#include "ecosenseatclock.h"
#include <Arduino.h>
#include "ecosense_main.h"

EcosenseAtClockFirmware::EcosenseAtClockFirmware() : IFirmware(){
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

ProcessFactory EcosenseAtClockFirmware::getFactory(int pId) {
	const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
		FACTORY(PRC_MAIN, MainProcess)
		,FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess)
		,FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess)
		,FACTORY(PRC_RTC, RTClockProcess)
		,FACTORY(PRC_WIFI, WifiProcess)
		//,FACTORY(PRC_BME280SENSOR, EnvironmentSensorsProcess),
	};
	int len = *(&factoryList + 1) - factoryList;	//TOTAL_FACTORIES_INCLUDED
	for (byte i = 0; i < len; i++) {
		if (factoryList[i].id == pId) {
			return factoryList[i].factory;
		}
	}
	return NULL;
}

static IFirmware* EcosenseAtClockFirmware::get() {
	if (IFirmware::instance == NULL) {
        		TRACELNF("new EcosenseAtClockFirmware");
		IFirmware::instance = new EcosenseAtClockFirmware();
	}
	return IFirmware::instance;
}
