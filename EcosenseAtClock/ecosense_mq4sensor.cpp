#include "ecosense_mq4sensor.h"
#include "ecosense_cfg.h"

MQ4SensorProcess::MQ4SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(MQ4_ANALOG_PIN_ADDR, pId, msg){
	/*pinMode(MQ4_ANALOG_PIN, INPUT);
	pinMode(MQ4_DIGITAL_PIN, INPUT) ;*/
	TRACELNF("MQ4SensorProcess::init");
}

static IFirmwareProcess* MQ4SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MQ4SensorProcess::factory");
	return new MQ4SensorProcess(pId, msg);
}
