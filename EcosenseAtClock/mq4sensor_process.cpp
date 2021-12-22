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
	return getSimpleResultMsg(AirQualityMsg::GasType::CH4, this->getVoltage());
}
