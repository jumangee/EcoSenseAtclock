#include "simple_sensor_process.h"
#include "ecosense_cfg.h"

SimpleSensorProcess::SimpleSensorProcess(uint16_t pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	readingsCount = 0;
            value = 0;
	TRACELNF("SimpleSensorProcess::init");
}
