#include "ecosense_mq7sensor.h"
#include "ecosense_cfg.h"

MQ7SensorProcess::MQ7SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(MQ7_ANALOG_PIN_ADDR, pId, msg){
	TRACELNF("MQ7SensorProcess::init");
}

static IFirmwareProcess* MQ7SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MQ7SensorProcess::factory");
	return new MQ7SensorProcess(pId, msg);
}
