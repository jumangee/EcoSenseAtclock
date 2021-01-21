#include "ecosense_mq4sensor.h"
#include "ecosense_cfg.h"
#include "mqsensor_process.h"

MQ4SensorProcess::MQ4SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(pId, msg){
	pinMode(MQ4_ANALOG_PIN, INPUT);
	pinMode(MQ4_DIGITAL_PIN, INPUT) ;
	TRACELNF("MQ4SensorProcess::init");
}

static IFirmwareProcess* MQ4SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MQ4SensorProcess::factory");
	return new MQ4SensorProcess(pId, msg);
}

void MQ4SensorProcess::update(unsigned long ms) {
	if (!readingsDone(MQ4_ANALOG_PIN, READINGS_PER_RESULT)) {
		return;
	}
	{
		TRACEF("[ MQ-4 ] analog=");
		TRACE(this->getValue());
		//TRACEF(", dig=");
		//TRACE(this->mq136dig);
		TRACEF(", V=");
		TRACE( this->getVoltage() );
		TRACEF(", instant=");
		TRACE( this->instantValue(MQ4_ANALOG_PIN) );
		TRACEF(", Dig=");
		TRACELN( digitalRead(MQ4_DIGITAL_PIN) );
	}
	this->getHost()->sendMessage(new AirQualityMsg(CH4, this->getQuality(.4), this->getVoltage()));
	this->pause(ENVSENSORS_TIMEOUT);
}
