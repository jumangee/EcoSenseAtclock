/**
 * MQ-7 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ7_SENSOR_PROCESS_H
#define _MQ7_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "mqsensor_process.h"

class MQ7SensorProcess: public MQSensorProcess {
	public:
		MQ7SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		IProcessMessage* getResultMsg() {
			return new AirQualityMsg(CO, this->getQuality(.4), this->getVoltage());
		}
};

#endif
