#include "meteo_mq136sensor.h"
#include "meteo_cfg.h"
#include "processy_cfg.h"
#include "mqsensor_process.h"

MQ136SensorProcess::MQ136SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(pId, msg){
	pinMode(MQ136_ANALOG_PIN, INPUT) ;
	//pinMode(MQ136_DIGITAL_PIN, INPUT) ;
	TRACELNF("MQ136SensorProcess::init");
}

static IFirmwareProcess* MQ136SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MQ136SensorProcess::factory");
	return new MQ136SensorProcess(pId, msg);
}

void MQ136SensorProcess::update(unsigned long ms) {
	if (!readingsDone(MQ136_ANALOG_PIN, READINGS_PER_RESULT)) {
		return;
	}
	TRACELNF("---[ MQ-136 ]---");
	{
		TRACEF("[RAW] analog=");
		TRACE(this->getValue());
		//TRACEF(", dig=");
		//TRACE(this->mq136dig);
		TRACEF(", V=");
		TRACELN( this->getVoltage() );
	}
	this->getHost()->sendMessage(new AirQualityMsg(H2S, this->getQuality(.6)));
	this->pause(ENVSENSORS_TIMEOUT);
}
