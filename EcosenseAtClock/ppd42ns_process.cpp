#include "ppd42ns_process.h"
#include "ecosense_messages.h"
#include "swserialsingleton.h"
#include "adcmux_mngmnt.h"

ParticlePPD42SensorProcess::ParticlePPD42SensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	TRACELNF("ParticlePPD42SensorProcess: pre-burn timeout")
	this->pause(60000);
}

static IFirmwareProcess* ParticlePPD42SensorProcess::factory(IProcessMessage* msg) {
	return new ParticlePPD42SensorProcess(msg);
}

void ParticlePPD42SensorProcess::done() {
	// calc average result
	float resultRatioPm1 = 0;
	float resultRatioPm2 = 0;
	float resultConcPm1 = 0;
	float resultConcPm2 = 0;
	for (byte i = 0; i < PPD42NS_READS_COUNT; i++) {
		resultRatioPm1 += samples[i].ratioPm1;
		resultRatioPm2 += samples[i].ratioPm2;
		resultConcPm1 += samples[i].concentrationPm1;
		resultConcPm2 += samples[i].concentrationPm2;
	}
	this->sendMessage(new ParticlePPD42Message(resultRatioPm1 / PPD42NS_READS_COUNT, resultConcPm1 / PPD42NS_READS_COUNT, resultRatioPm2 / PPD42NS_READS_COUNT, resultConcPm2 / PPD42NS_READS_COUNT));
	this->sendMessage(new TaskDoneMessage(this));
	this->pause();
}

void ParticlePPD42SensorProcess::update(unsigned long ms) {
	if (!SoftwareSerialSingleton::isfree()) {	// better not to test in parallel with intercept-blocking operations
		pause(250);
		return;
	}
	if (this->currentMeasureMode == 0) {
		this->currentMeasureMode = MUXCHANNEL_PM1;
	}
	uint32_t ticket = ADCMuxManagement::get()->requestChannel(this->currentMeasureMode);	// ADC MUX PIN!
	if (ticket) {
		unsigned long starttime = millis();
		this->lowpulseoccupancy += pulseIn(this->currentMeasureMode, LOW, 1000);
		ADCMuxManagement::get()->release(ticket);
		this->sampleTotalTime += millis()-starttime;
		if ((this->sampleTotalTime) > PPD42NS_SAMPLE_TIME_MS)
		{
			float ratio = this->lowpulseoccupancy/(PPD42NS_SAMPLE_TIME_MS*10.0);  // Integer percentage 0=>100
			float concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
			
			/*TRACEF("mode: ");
			TRACE(this->currentMeasureMode);
			TRACEF(", lowpulseoccupancy: ");
			TRACE(lowpulseoccupancy);
			
			TRACEF(", ratio: ");
			TRACE(ratio);
			TRACEF(", concentration: ");
			TRACELN(concentration);*/
			if (this->currentMeasureMode == MUXCHANNEL_PM1) {
				samples[this->currentSample].ratioPm1 = ratio;
				samples[this->currentSample].concentrationPm1 = concentration;
				this->currentMeasureMode = MUXCHANNEL_PM2;
			} else {
				samples[this->currentSample].ratioPm2 = ratio;
				samples[this->currentSample].concentrationPm2 = concentration;
				this->currentMeasureMode = MUXCHANNEL_PM1;
				this->currentSample++;
				if (this->currentSample >= PPD42NS_READS_COUNT) {
					this->currentSample = 0;
					//stop sampling and process
					this->done();
				}
			}
			this->lowpulseoccupancy = 0;
			this->sampleTotalTime = 0;
		}
	}
}
