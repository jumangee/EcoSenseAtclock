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
	#if NOWIFI_BUILD != 1
	addProcess(PRC_WIFI);
	#endif
	addProcess(PRC_BME280);
	addProcess(PRC_BTN);
	addProcess(PRC_MHZ19);
	#if SLIM_BUILD != 1
	PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));	//EcosenseAtClockFirmware::PwrMngmtPins, (*(&PwrMngmtPins + 1) - PwrMngmtPins)
	ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);

	addProcess(PRC_CONSUMER1);
	#endif
};

ProcessFactory EcosenseAtClockFirmware::getFactory(uint16_t pId) {
	const static ProcessFactoryReg factoryList[] = {	//	factories list	//TOTAL_FACTORIES_INCLUDED
		FACTORY(DisplayProcess)
		,FACTORY(RTClockProcess)
		#if NOWIFI_BUILD != 1
		,FACTORY(WifiProcess)
		#endif
		,FACTORY(BME280SensorProcess)
		,FACTORY(ButtonSensorProcess)
		,FACTORY(MHZ19SensorProcess)
		#if SLIM_BUILD != 1
		,FACTORY(PwrConsumer1Process)
		,FACTORY(PwrConsumer2Process)
		//,FACTORY(PwrConsumer3Process)
		,FACTORY(MQ136SensorProcess)
		//,FACTORY(MQ7SensorProcess)
		,FACTORY(MQ135SensorProcess)
		,FACTORY(MQ4SensorProcess)
		//,FACTORY(ParticlePPD42SensorProcess)
		//,FACTORY(ZE08CH2OSensorProcess)
		,FACTORY(CJMCU1100SensorProcess)
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
