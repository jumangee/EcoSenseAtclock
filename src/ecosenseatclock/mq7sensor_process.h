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
            this->pause(MQ7_PREBURN_TIMEOUT);    // pre-burn timeout
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MQ7SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		IProcessMessage* getResultMsg() {
			return getSimpleResultMsg(AirQualityMsg::GasType::CO, this->getVoltage());
		}
};

#endif
