#include "mq135sensor_process.h"
#include "ecosense_cfg.h"
#include "ecosense_messages.h"

MQ135SensorProcess::MQ135SensorProcess(IProcessMessage* msg) : MQSensorProcess(MUXCHANNEL_MQ135, msg){
            this->pause(30000);    // pre-burn timeout
}

static IFirmwareProcess* MQ135SensorProcess::factory(IProcessMessage* msg) {
	return new MQ135SensorProcess(msg);
}

IProcessMessage* MQ135SensorProcess::getResultMsg() {
	return getSimpleResultMsg(AirQualityMsg::GasType::COMMON, this->getVoltage());
}
