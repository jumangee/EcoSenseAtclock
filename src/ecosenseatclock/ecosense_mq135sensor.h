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
		//@implement
		//@include "ecosense_cfg.h"
		MQ135SensorProcess(int pId, IProcessMessage* msg): MQSensorProcess(MQ135_ANALOG_PIN_ADDR, pId, msg) {
			TRACELNF("MQ135SensorProcess::init");
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("MQ135SensorProcess::factory");
			return new MQ135SensorProcess(pId, msg);
		}

		IProcessMessage* getResultMsg() {
			return new AirQualityMsg(COMMON, this->getQuality(.4), this->getVoltage());
		}
};

#endif
