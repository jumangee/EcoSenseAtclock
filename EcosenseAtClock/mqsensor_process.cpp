#include "mqsensor_process.h"
#include "ecosense_cfg.h"
#include <Arduino.h>

MQSensorProcess::MQSensorProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
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
