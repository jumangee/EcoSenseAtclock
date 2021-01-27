#include "ecosense_cjmcu1100sensor.h"
#include "ecosense_cfg.h"

CJMCU1100SensorProcess::CJMCU1100SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(CJMCU1100_ANALOG_PIN_ADDR, pId, msg){
	TRACELNF("CJMCU1100SensorProcess::init");
}

static IFirmwareProcess* CJMCU1100SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("CJMCU1100SensorProcess::factory");
	return new CJMCU1100SensorProcess(pId, msg);
}
