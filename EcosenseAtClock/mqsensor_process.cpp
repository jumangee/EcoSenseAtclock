#include "mqsensor_process.h"
#include "ecosense_cfg.h"
#include <Arduino.h>

MQSensorProcess::MQSensorProcess(uint16_t pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	analogReference(EXTERNAL);
	readingsCount = 0;
            value = 0;
	preHeated = false;
	startTime = millis();
	this->pause(PREHEAT_TIME);
	TRACELNF("MQSensorProcess::init");
}

MQSensorProcess::~MQSensorProcess() {
	// stop process
	TRACELNF("MQSensorProcess::stop");
}
