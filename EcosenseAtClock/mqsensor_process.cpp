#include "mqsensor_process.h"
#include "ecosense_cfg.h"

MQSensorProcess::MQSensorProcess(byte pin, uint16_t pId, IProcessMessage* msg) : SimpleSensorProcess(pId, msg){
	/*readingsCount = 0;
            value = 0;
	preHeated = false;
	startTime = millis();
	this->pause(PREHEAT_TIME);*/
	this->pin = pin;
	TRACELNF("MQSensorProcess::init");
}

void MQSensorProcess::update(unsigned long ms) {
	uint32_t ticket = ADCMuxManagement::get()->requestPin(this->pin);	// ADC MUX PIN!
	if (ticket) {
		{
			bool done = this->readingsDone(READINGS_PER_RESULT);
			ADCMuxManagement::get()->releasePin(ticket);
			if (!done) {
				return;
			}
		}
		IProcessMessage* result = this->getResultMsg();
		if (result) {
			this->getHost()->sendMessage(result);
		}
		
	}
	this->pause(ENVSENSORS_TIMEOUT);
}
