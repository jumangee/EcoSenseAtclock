#include "ecosense_mq135sensor.h"
#include "ecosense_cfg.h"

MQ135SensorProcess::MQ135SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(MQ135_ANALOG_PIN_ADDR, pId, msg){
	TRACELNF("MQ135SensorProcess::init");
}

static IFirmwareProcess* MQ135SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MQ135SensorProcess::factory");
	return new MQ135SensorProcess(pId, msg);
}
