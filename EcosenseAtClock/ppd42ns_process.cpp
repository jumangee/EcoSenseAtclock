#include "ppd42ns_process.h"
#include "swserialsingleton.h"
#include "ecosense_messages.h"

PPD42SensorProcess::PPD42SensorProcess(IProcessMessage* msg) : ADCMuxChannelProcess(msg){
	this->pause(PPD42NS_PREBURN_TIMEOUT);
	TRACELNF("PPD42: pre-burn timeout")
}

static IFirmwareProcess* PPD42SensorProcess::factory(IProcessMessage* msg) {
	return new PPD42SensorProcess(msg);
}

void PPD42SensorProcess::update(unsigned long ms) {
	if (!SoftwareSerialSingleton::isfree()) {	// better not to test in parallel with intercept-blocking operations
		pause(250);
		return;
	}
	if (getConcentration(MUXCHANNEL_PM2)) {
		this->sendMessage(new AirQualityMsg(AirQualityMsg::GasType::PM25, AirQualityMsg::value2code(ratio/20), concentration ));
		this->sendMessage(new TaskDoneMessage(this));
		this->pause();
	}
}
