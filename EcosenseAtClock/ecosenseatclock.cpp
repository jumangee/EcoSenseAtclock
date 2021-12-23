#include "ecosenseatclock.h"
#include <Arduino.h>

const static byte EcosenseAtClockFirmware::PwrMngmtPins[] = PWRMNGMTPINS;
const static byte EcosenseAtClockFirmware::AdcMuxMngmtPins[] = ADCMUXPINS;
EcosenseAtClockFirmware::EcosenseAtClockFirmware() : IFirmware(){
	#if PROCESSY_DEBUG_SERIAL == 1
	Serial.begin(9600);
	delay(1000);		
	TRACELNF("START");
	#endif
	analogReference(EXTERNAL);	// important!
	addProcess(PRC_DISPLAY);
	addProcess(PRC_RTC);
	addProcess(PRC_WIFI);
	addProcess(PRC_BME280);
	addProcess(PRC_BTN);
	#if SLIM_BUILD != 1
	PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));	//EcosenseAtClockFirmware::PwrMngmtPins, (*(&PwrMngmtPins + 1) - PwrMngmtPins)
	TRACELNF("Power management: started");
	ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);
	TRACELNF("AdcMux management: started");
	
	addProcess(PRC_CONSUMER1);	//, ProcessOrderMessage::start()
	#endif
};

ProcessFactory EcosenseAtClockFirmware::getFactory(uint16_t pId) {
	const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
		FACTORY(DisplayProcess)
		,FACTORY(RTClockProcess)
		,FACTORY(WifiProcess)
		,FACTORY(BME280SensorProcess)
		,FACTORY(ButtonSensorProcess)
		#if SLIM_BUILD != 1
		,FACTORY(PwrConsumer1Process)
		,FACTORY(PwrConsumer2Process)
		,FACTORY(PwrConsumer3Process)
		,FACTORY(MQ136SensorProcess)
		//,FACTORY(MQ7SensorProcess)
		,FACTORY(MQ135SensorProcess)
		,FACTORY(MQ4SensorProcess)
		//,FACTORY(PPD42SensorProcess)
		//,FACTORY(ZE08CH2OSensorProcess)
		,FACTORY(CJMCU1100SensorProcess)
		,FACTORY(MHZ19SensorProcess)
		#endif
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
		IFirmware::instance = new EcosenseAtClockFirmware();
	}
	return IFirmware::instance;
}
