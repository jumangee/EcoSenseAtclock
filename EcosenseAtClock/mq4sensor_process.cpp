#include "mq4sensor_process.h"
#include "ecosense_cfg.h"
#include "ecosense_messages.h"

MQ4SensorProcess::MQ4SensorProcess(IProcessMessage* msg) : MQSensorProcess(MUXCHANNEL_MQ4, msg){
            this->pause(30000);    // pre-burn timeout
}

static IFirmwareProcess* MQ4SensorProcess::factory(IProcessMessage* msg) {
	return new MQ4SensorProcess(msg);
}

IProcessMessage* MQ4SensorProcess::getResultMsg() {
	return new AirQualityMsg(AirQualityMsg::GasType::CH4, AirQualityMsg::value2code(int(this->getVoltage())), int(this->getVoltage()));
	//return new AirQualityMsg(CH4, this->getQuality(.4), this->getVoltage());
}
