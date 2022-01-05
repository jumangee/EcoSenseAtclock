#include "mq136sensor_process.h"
#include "ecosense_messages.h"

MQ136SensorProcess::MQ136SensorProcess(IProcessMessage* msg) : MQSensorProcess(MUXCHANNEL_MQ136, msg){
            this->pause(MQ136_PREBURN_TIMEOUT);    // pre-burn timeout
	TRACELNF("MQ136: pre-burn timeout")
}

static IFirmwareProcess* MQ136SensorProcess::factory(IProcessMessage* msg) {
	return new MQ136SensorProcess(msg);
}

IProcessMessage* MQ136SensorProcess::getResultMsg() {
	return getSimpleResultMsg(AirQualityMsg::GasType::H2S, this->getVoltage());
}
