/**
 * MQ-4 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ4_SENSOR_PROCESS_H
#define _MQ4_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "mqsensor_process.h"

//#define MQ136_RL 2.3
//#define MQ136_FRESH_RO 1

class MQ4SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ4);
		
		MQ4SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		IProcessMessage* getResultMsg();

};

#endif
