#include "ecosenseatclock.h"
#include <Arduino.h>
#include "stuff.h"
#include "MemoryFree.h"

const static byte EcosenseAtClockFirmware::PwrMngmtPins[] = PWRMNGMTPINS;
const static byte EcosenseAtClockFirmware::AdcMuxMngmtPins[] = ADCMUXPINS;
EcosenseAtClockFirmware::EcosenseAtClockFirmware() : IFirmware(){
	#if PROCESSY_DEBUG_SERIAL == 1
	Serial.begin(9600);
	delay(1000);		
	TRACELNF("START");
	#endif
	analogReference(EXTERNAL);	// important!
	// ---[ TASK REGISTRATION ]---
	PROCESS_REG(DisplayProcess);
	PROCESS_REG(RTClockProcess);
	#if NOWIFI_BUILD != 1
		PROCESS_REG(WifiProcess);
	#endif
	PROCESS_REG(BME280SensorProcess);
	PROCESS_REG(EcoSenseAtClockBtnProcess);
	PROCESS_REG(MHZ19SensorProcess);
	#if SLIM_BUILD != 1
		PROCESS_REG(PwrConsumer1Process);
		PROCESS_REG(PwrConsumer2Process);
		PROCESS_REG(PwrConsumer3Process);
		PROCESS_REG(MQ136SensorProcess);
		PROCESS_REG(MQ7SensorProcess);
		PROCESS_REG(MQ135SensorProcess);
		PROCESS_REG(MQ4SensorProcess);
		PROCESS_REG(PPD42SensorProcess);
		PROCESS_REG(CJMCU1100SensorProcess);
	#endif
	// ---[ STARTUP ]---
	addProcess(PRC_DISPLAY);
	addProcess(PRC_RTC);
	#if NOWIFI_BUILD != 1
		addProcess(PRC_WIFI);
	#endif
	addProcess(PRC_BME280);
	addProcess(PRC_BTN);
	addProcess(PRC_MHZ19);
	#if SLIM_BUILD != 1
		PowerloadManagement::init(ARR2PTR(EcosenseAtClockFirmware::PwrMngmtPins));
		ADCMuxManagement::init(EcosenseAtClockFirmware::AdcMuxMngmtPins);
	
		addProcess(PRC_CONSUMER1);
	#endif
};

static IFirmware* EcosenseAtClockFirmware::get() {
	if (IFirmware::instance == NULL) {
		IFirmware::instance = new EcosenseAtClockFirmware();
	}
	return IFirmware::instance;
}

void EcosenseAtClockFirmware::handlerProcessDebugTimer(unsigned long dT) {
	byte processCount = 0;
	for (uint16_t i = 0; i < this->processListSize; i++) {
		IFirmwareProcessRegistration* reg = this->processList[i];
		if (reg->isActive()) {
			processCount++;
		}
	}
	this->sendMessage(new SelfReportMessage(processCount, freeMemory()));
	TRACEF("SYS REPORT: processes=")
	TRACE(processCount)
	TRACEF(", freemem=")
	TRACELN(freeMemory())
}
