#include "mq7sensor_process.h"
#include "ecosense_messages.h"

static IFirmwareProcess* MQ7SensorProcess::factory(IProcessMessage* msg) {
	return new MQ7SensorProcess(msg);
}

IProcessMessage* MQ7SensorProcess::getResultMsg() {
	return getSimpleResultMsg(AirQualityMsg::GasType::CO, this->getVoltage());
}
