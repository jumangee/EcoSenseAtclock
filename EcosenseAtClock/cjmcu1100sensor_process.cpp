#include "cjmcu1100sensor_process.h"
#include "ecosense_cfg.h"
#include "ecosense_messages.h"

CJMCU1100SensorProcess::CJMCU1100SensorProcess(IProcessMessage* msg) : MQSensorProcess(MUXCHANNEL_CJMCU1100, msg){
            this->pause(CJMCU1100_PREBURN_TIMEOUT);    // pre-burn timeout
	TRACELNF("CJMCU1100: pre-burn timeout")
}

static IFirmwareProcess* CJMCU1100SensorProcess::factory(IProcessMessage* msg) {
	return new CJMCU1100SensorProcess(msg);
}

IProcessMessage* CJMCU1100SensorProcess::getResultMsg() {
	float v = this->getVoltage();
	float ppm = max(
		exp( (-2.631) + 1.528 * v + (-1.125) * pow(v, 2) ),		// formaldehyde	// 0.01mg/dm3 - critical, so, 10ppm is critial (10mg/m3)
		exp( (-7.071) + 2.852 * v + (-0.210) * pow(v, 2) )		// toluene
	);
	return new AirQualityMsg(AirQualityMsg::GasType::VOCs, AirQualityMsg::value2code(round(min(ppm*.7, 10.0) / 2)), ppm);
}
