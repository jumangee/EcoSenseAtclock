/**
 * MQ-7 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ7_SENSOR_PROCESS_H
#define _MQ7_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "mqsensor_process.h"

class MQ7SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ7);
		
		MQ7SensorProcess(IProcessMessage* msg): MQSensorProcess(MUXCHANNEL_MQ7, msg) {
            this->pause(30000);    // pre-burn timeout
		}

		static IFirmwareProcess* factory(IProcessMessage* msg);

		IProcessMessage* getResultMsg();
};

#endif
