#include "meteo.h"
#include <Arduino.h>
#include "meteo_main.h"

MeteoClockFirmware::MeteoClockFirmware() : IFirmware(){
	#ifdef DEBUG_SERIAL
	TRACELNF("START");
	#endif
	{
		String s = SF(PRC_MAIN);
		this->addProcess(s);
	}
	{
		String s = SF(PRC_SENSORS);
		this->addProcess(s);
	}
};

ProcessFactory MeteoClockFirmware::getFactory(String & name) {
	String PRC_MAINs = SF(PRC_MAIN);
	String PRC_SENSORSs = SF(PRC_SENSORS);
	const static byte size PROGMEM = {2};
	const static ProcessFactoryReg factoryList[size] = {
		FACTORY(PRC_MAINs, MainProcess),
		FACTORY(PRC_SENSORSs, EnvironmentSensorsProcess)
	};
	for (byte i = 0; i < size; i++) {
		if (factoryList[i].id.equals(name)) {
			return factoryList[i].factory;
		}
	}
	return NULL;
}

static IFirmware* MeteoClockFirmware::get() {
	if (IFirmware::instance == NULL) {
        		TRACELNF("new MeteoClockFirmware");
		IFirmware::instance = new MeteoClockFirmware();
		//((MeteoClockFirmware*)IFirmware::instance)->init();
	}
	return IFirmware::instance;
}
