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
		
		//@implement
		//@include "ecosense_cfg.h"
		MQ135SensorProcess(IProcessMessage* msg): MQSensorProcess(MUXCHANNEL_MQ135, msg) {
            this->pause(30000);    // pre-burn timeout
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MQ135SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		IProcessMessage* getResultMsg() {
			uint8_t res = round(this->getVoltage());
			return new AirQualityMsg(AirQualityMsg::GasType::COMMON, AirQualityMsg::value2code(res), res);
			//return new AirQualityMsg(COMMON, this->getQuality(.4), this->getVoltage());
		}
};

#endif
