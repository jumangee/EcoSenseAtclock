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
		
		//@implement
		MQ136SensorProcess(IProcessMessage* msg): MQSensorProcess(MUXCHANNEL_MQ136, msg) {
            this->pause(MQ136_PREBURN_TIMEOUT);    // pre-burn timeout
			TRACELNF("MQ136: pre-burn timeout")
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MQ136SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		IProcessMessage* getResultMsg() {
			return getSimpleResultMsg(AirQualityMsg::GasType::H2S, this->getVoltage());
		}

};

#endif
