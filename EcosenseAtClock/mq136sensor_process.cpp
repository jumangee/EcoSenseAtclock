#include "mq136sensor_process.h"
#include "ecosense_messages.h"

MQ136SensorProcess::MQ136SensorProcess(IProcessMessage* msg) : MQSensorProcess(msg){
            this->pause(30000);    // pre-burn timeout
}

static IFirmwareProcess* MQ136SensorProcess::factory(IProcessMessage* msg) {
	return new MQ136SensorProcess(msg);
}

IProcessMessage* MQ136SensorProcess::getResultMsg() {
	return new AirQualityMsg(AirQualityMsg::GasType::H2S, AirQualityMsg::value2code(int(this->getVoltage())), int(this->getVoltage()));
	//this->getQuality(.6), this->getVoltage()
}
