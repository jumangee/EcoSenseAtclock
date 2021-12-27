#include "mqsensor_process.h"
#include "ecosense_cfg.h"
#include "ecosense_messages.h"

MQSensorProcess::MQSensorProcess(byte channel, IProcessMessage* msg) : SimpleSensorProcess(msg){
	this->channel = channel;
}

uint16_t MQSensorProcess::getInstantValue() {
            return analogRead( ADCMUX_SIGNAL_PIN );
        }

void MQSensorProcess::update(unsigned long ms) {
	//uint32_t ticket = ADCMuxManagement::get()->requestChannel(this->channel);	// ADC MUX PIN!
	if (this->getChannel(this->channel)) {
		bool done = this->readingsDone(MQ_READINGS_PER_RESULT);
		//ADCMuxManagement::get()->release(ticket);
		this->freeChannel();
		if (done) {
			TRACELNF("MQSensorProcess:done!")
			IProcessMessage* result = this->getResultMsg();
			if (result) {
				this->sendMessage(result);
				this->sendMessage(new TaskDoneMessage(this));
			}
			this->pause();
			return;
		}
	}
	TRACEF("MQSensor: ")
	TRACE(this->getId())
	TRACEF(" updated: ")
	TRACELN(getValue())
	this->pause(MQ_READING_TIMEOUT);
}
