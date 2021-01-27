#include "ecosense_monitor_process.h"
#include "ecosense_cfg.h"

DataMonitorProcess::DataMonitorProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
}

void DataMonitorProcess::update(unsigned long ms) {
	this->pause(ENVSENSORS_TIMEOUT);	// no need to be updated
}

static IFirmwareProcess* DataMonitorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("DataMonitorProcess::factory");
	return new DataMonitorProcess(pId, msg);
}

bool DataMonitorProcess::handleMessage(IProcessMessage* msg) {
	switch (msg->getType())
	{
		case ENVDATA_MESSAGE: {
			this->handleEnvDataMsg((EnvDataMessage*)msg);
			break;
		}
		case AIRQUALITY_MESSAGE: {
			this->handleAirQualityMsg((AirQualityMsg*)msg);
			break;
		}
	}
	return false;
}
