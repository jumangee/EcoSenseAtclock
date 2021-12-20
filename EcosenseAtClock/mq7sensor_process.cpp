#include "mq7sensor_process.h"
#include "ecosense_messages.h"

static IFirmwareProcess* MQ7SensorProcess::factory(IProcessMessage* msg) {
	return new MQ7SensorProcess(msg);
}

IProcessMessage* MQ7SensorProcess::getResultMsg() {
	return new AirQualityMsg(AirQualityMsg::GasType::CO, AirQualityMsg::value2code(int(this->getVoltage())), int(this->getVoltage()));
	//return new AirQualityMsg(CO, this->getQuality(.4), this->getVoltage());
}
