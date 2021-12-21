#include "cjmcu1100sensor_process.h"
#include "ecosense_cfg.h"
#include "ecosense_messages.h"

CJMCU1100SensorProcess::CJMCU1100SensorProcess(IProcessMessage* msg) : MQSensorProcess(MUXCHANNEL_CJMCU1100, msg){
            this->pause(30000);    // pre-burn timeout
	TRACELNF("CJMCU1100SensorProcess: pre-burn timeout")
}

static IFirmwareProcess* CJMCU1100SensorProcess::factory(IProcessMessage* msg) {
	return new CJMCU1100SensorProcess(msg);
}

IProcessMessage* CJMCU1100SensorProcess::getResultMsg() {
	float v = this->getVoltage();
	// One parts per million is equal to 0.998859 milligrams per liter
	// 0.01mg/dm3 - critical, so, 10ppm is critial (10mg/m3)
	float logPPM = (-2.631) + 1.528 * v + (-1.125) * pow(v, 2);
	TRACELNF("CJMCU1100SensorProcess:getResultMsg")
	return new AirQualityMsg(AirQualityMsg::GasType::VOCs, AirQualityMsg::value2code(byte(expf( logPPM ))), round(logPPM));
	//return new AirQualityMsg(VOCs, byte(expf( logPPM )), v);
}
