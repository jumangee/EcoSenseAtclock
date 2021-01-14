#include "mqsensor_process.h"
#include "meteo_cfg.h"
#include <Arduino.h>

MQSensorProcess::MQSensorProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	readingsCount = 0;
            value = 0;
	TRACELNF("MQSensorProcess::init");
}

MQSensorProcess::~MQSensorProcess() {
	// stop process
	TRACELNF("MQSensorProcess::stop");
}
