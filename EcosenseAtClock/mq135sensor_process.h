/**
 * MQ-135 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ135_SENSOR_PROCESS_H
#define _MQ135_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "mqsensor_process.h"

class MQ135SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ135);
		
		MQ135SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		IProcessMessage* getResultMsg();
};

#endif
