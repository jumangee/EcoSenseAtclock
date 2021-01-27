#include "ecosense_mq136sensor.h"
#include "ecosense_cfg.h"

MQ136SensorProcess::MQ136SensorProcess(int pId, IProcessMessage* msg) : MQSensorProcess(MQ136_ANALOG_PIN_ADDR, pId, msg){
	/*pinMode(MQ136_ANALOG_PIN, INPUT) ;
	pinMode(MQ136_DIGITAL_PIN, INPUT) ;*/
	TRACELNF("MQ136SensorProcess::init");
}

static IFirmwareProcess* MQ136SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MQ136SensorProcess::factory");
	return new MQ136SensorProcess(pId, msg);
}
