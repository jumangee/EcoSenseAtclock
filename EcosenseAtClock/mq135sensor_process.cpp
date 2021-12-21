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
	uint8_t res = round(this->getVoltage());
	return new AirQualityMsg(AirQualityMsg::GasType::COMMON, AirQualityMsg::value2code(res), res);
	//return new AirQualityMsg(COMMON, this->getQuality(.4), this->getVoltage());
}
