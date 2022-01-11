/**
 * MQ-136 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ136_SENSOR_PROCESS_H
#define _MQ136_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "mqsensor_process.h"

class MQ136SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ136);
		
		MQ136SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		IProcessMessage* getResultMsg();

};

#endif
