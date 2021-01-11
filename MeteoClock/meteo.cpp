#include "meteo.h"
#include <Arduino.h>
#include "processy_cfg.h"

MeteoClockFirmware::MeteoClockFirmware() : IFirmware(){
	#ifdef DEBUG_SERIAL
	Serial.println("START");
	#endif
};

		//!@include "meteo_main.h"
void MeteoClockFirmware::init() {
	FACTORY_DEFAULT(PRC_MAIN, MainProcess)
	//FACTORY(PRC_SENSORS, EnvironmentSensorsProcess)
	//FACTORY_DEFAULT(PRC_I2CSCANNER, i2cPortScannerProcess)
	//this->registerFactory(PRC_MAIN, [](String id, IProcessMessage* msg){return new MainProcess(id, msg);}, true);
	
	//this->addProcess(PRC_MAIN);
	//this->addProcess(PRC_SENSORS);
}

void MeteoClockFirmware::log(String msg) {
	#ifdef DEBUG_SERIAL
	Serial.println(msg);
	#endif
}

static IFirmware* MeteoClockFirmware::get() {
	if (IFirmware::instance == NULL) {
        		Serial.println("new MeteoClockFirmware");
		IFirmware::instance = new MeteoClockFirmware();
		((MeteoClockFirmware*)IFirmware::instance)->init();
	}
	return IFirmware::instance;
}
