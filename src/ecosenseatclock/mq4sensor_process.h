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

class MQ4SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ4);
		
		//@implement
		//@include "ecosense_cfg.h"
		MQ4SensorProcess(IProcessMessage* msg): MQSensorProcess(MUXCHANNEL_MQ4, msg) {
            this->pause(MQ4_PREBURN_TIMEOUT);    // pre-burn timeout
			TRACELNF("MQ4: pre-burn timeout")
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MQ4SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		IProcessMessage* getResultMsg() {
			return getSimpleResultMsg(AirQualityMsg::GasType::CH4, this->getVoltage());
		}

};

#endif
