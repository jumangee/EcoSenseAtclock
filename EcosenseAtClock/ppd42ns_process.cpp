#include "ppd42ns_process.h"
#include "swserialsingleton.h"
#include "adcmux_mngmnt.h"
#include "ecosense_messages.h"

ParticlePPD42SensorProcess::ParticlePPD42SensorProcess() : ADCMuxChannelProcess(NULL){
	TRACELNF("ParticlePPD42SensorProcess: pre-burn timeout")
	this->pause(60000);
}

static IFirmwareProcess* ParticlePPD42SensorProcess::factory(IProcessMessage* msg) {
	return new ParticlePPD42SensorProcess();
}

void ParticlePPD42SensorProcess::update(unsigned long ms) {
	if (!SoftwareSerialSingleton::isfree()) {	// better not to test in parallel with intercept-blocking operations
		pause(250);
		return;
	}
	byte pin = statePm1 ? MUXCHANNEL_PM1 : MUXCHANNEL_PM2;
	//uint32_t ticket = ADCMuxManagement::get()->requestChannel(pin);	// ADC MUX PIN!
	if (this->getChannel(pin)) {
		unsigned long starttime = millis();
		this->lowpulseoccupancy += pulseIn(pin, LOW, 1000);
		//ADCMuxManagement::get()->release(ticket);
		this->freeChannel();
		this->sampleTotalTime += millis()-starttime;
		if ((this->sampleTotalTime) > PPD42NS_SAMPLE_TIME_MS)
		{
			float ratio = this->lowpulseoccupancy/(PPD42NS_SAMPLE_TIME_MS*10.0);  // Integer percentage 0=>100
			float concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
			if (statePm1) {
				// pm1
				ratioPm1 = ratio;
				concentrationPm1 = concentration;
			} else {
				// pm2
				ratioPm2 = ratio;
				concentrationPm2 = concentration;
				this->sendMessage(new AirQualityMsg(AirQualityMsg::GasType::PM25, AirQualityMsg::value2code(ratioPm1/20), concentrationPm2 ));
				//this->sendMessage(new AirQualityMsg(AirQualityMsg::GasType::PM1, AirQualityMsg::value2code(ratioPm1/20), concentrationPm1 ));
				this->sendMessage(new TaskDoneMessage(this));
				this->pause();
			}
			this->lowpulseoccupancy = 0;
			this->sampleTotalTime = 0;
		}
	}
}
