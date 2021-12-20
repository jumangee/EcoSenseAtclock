#include "simplesensor_process.h"

SimpleSensorProcess::SimpleSensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	this->readingsCount = 0;
            this->value = 0;
}
