#include "mqsensor_process.h"
#include "ecosense_cfg.h"
#include "ecosense_messages.h"

MQSensorProcess::MQSensorProcess(IProcessMessage* msg) : SimpleSensorProcess(msg){
}

void MQSensorProcess::update(unsigned long ms) {
	uint32_t ticket = ADCMuxManagement::get()->requestPin();	// ADC MUX PIN!
	if (ticket) {
		bool done = this->readingsDone(MQ_READINGS_PER_RESULT);
		ADCMuxManagement::get()->release(ticket);
		if (done) {
			IProcessMessage* result = this->getResultMsg();
			if (result) {
				this->sendMessage(result);
				this->sendMessage(new TaskDoneMessage(this));
			}
		}
	}
	this->pause(MQ_READING_TIMEOUT);
}
