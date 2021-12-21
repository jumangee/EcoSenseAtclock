#include "mq136sensor_process.h"
#include "ecosense_messages.h"

MQ136SensorProcess::MQ136SensorProcess(IProcessMessage* msg) : MQSensorProcess(MUXCHANNEL_MQ136, msg){
            this->pause(30000);    // pre-burn timeout
}

static IFirmwareProcess* MQ136SensorProcess::factory(IProcessMessage* msg) {
	return new MQ136SensorProcess(msg);
}

IProcessMessage* MQ136SensorProcess::getResultMsg() {
	uint8_t res = round(this->getVoltage());
	return new AirQualityMsg(
		AirQualityMsg::GasType::H2S, 
		res < 1.5 ? AirQualityMsg::GasConcentration::MINIMAL : (
			res < 3 ? AirQualityMsg::GasConcentration::NORM : (
				res < 4 ? AirQualityMsg::GasConcentration::WARNING : AirQualityMsg::GasConcentration::DANGER
			)
		),
		res
	);
}
