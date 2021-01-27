#include "ecosenseatclock.h"
#include <Arduino.h>

const static byte EcosenseAtClockFirmware::PwrMngmtPins[] = PWRMNGMTPINS;
const static byte EcosenseAtClockFirmware::AdcMuxMngmtPins[] = ADCMUXPINS;
EcosenseAtClockFirmware::EcosenseAtClockFirmware() : IFirmware(){
	#ifdef PROCESSY_DEBUG_SERIAL
	Serial.begin(9600);
	delay(1000);		
	TRACELNF("START");
	#endif
	analogReference(EXTERNAL);	// important!
	addProcess(PRC_MAIN);
	//addProcess(PRC_MQ136SENSOR);
	//addProcess(PRC_MQ4SENSOR);
	addProcess(PRC_RTC);
	addProcess(PRC_WIFI);
	addProcess(PRC_BME280SENSOR);
	PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));	//EcosenseAtClockFirmware::PwrMngmtPins, (*(&PwrMngmtPins + 1) - PwrMngmtPins)
	TRACELNF("Power management DONE");
	ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);
	TRACELNF("AdcMux management DONE");
	
	addProcess(PRC_CONSUMER1);
	addProcess(PRC_CONSUMER2);
	addProcess(PRC_CONSUMER3);
	this->sendMessage(ProcessOrderMessage::start());
};

		//!@include "ecosense_main.h"
ProcessFactory EcosenseAtClockFirmware::getFactory(uint16_t pId) {
	const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
		FACTORY(PRC_MAIN, MainProcess)
		,FACTORY(PRC_RTC, RTClockProcess)
		,FACTORY(PRC_WIFI, WifiProcess)
		,FACTORY(PRC_BME280SENSOR, EnvironmentSensorsProcess),
		FACTORY(PRC_CONSUMER1, PwrConsumer1Process)
		,FACTORY(PRC_CONSUMER2, PwrConsumer2Process)
		,FACTORY(PRC_CONSUMER3, PwrConsumer3Process)
		,FACTORY(PRC_MQ136SENSOR, MQ136SensorProcess)
		,FACTORY(PRC_MQ7SENSOR, MQ7SensorProcess)
		,FACTORY(PRC_MQ135SENSOR, MQ135SensorProcess)
		,FACTORY(PRC_MQ4SENSOR, MQ4SensorProcess)
		,FACTORY(PRC_PPD42SENSOR, PPD42SensorProcess)
		,FACTORY(PRC_ZE08SENSOR, ZE08CH2OSensorProcess)
		,FACTORY(PRC_CJMCU1100SENSOR, CJMCU1100SensorProcess)
		,FACTORY(PRC_MHZ19SENSOR, MHZ19SensorProcess)
		//,FACTORY(PRC_DUMB1, Dumb1Process)
		//,FACTORY(PRC_DUMB2, Dumb2Process)
		//,FACTORY(PRC_DUMB3, Dumb1Process)
		//,FACTORY(PRC_DUMB4, Dumb2Process)
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
