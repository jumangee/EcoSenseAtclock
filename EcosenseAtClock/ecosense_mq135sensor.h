/**
 * MQ-135 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ135_SENSOR_PROCESS_H
#define _MQ135_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "mqsensor_process.h"

class MQ135SensorProcess: public MQSensorProcess {
	public:
		MQ135SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		IProcessMessage* getResultMsg() {
			return new AirQualityMsg(COMMON, this->getQuality(.4), this->getVoltage());
		}
};

#endif
