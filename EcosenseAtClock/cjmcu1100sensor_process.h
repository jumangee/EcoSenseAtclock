/**
 * CJMCU-1100 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _CJMCU1100_SENSOR_PROCESS_H
#define _CJMCU1100_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "mqsensor_process.h"

class CJMCU1100SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_CJMCU1100);
		
		CJMCU1100SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		IProcessMessage* getResultMsg();

};

#endif
