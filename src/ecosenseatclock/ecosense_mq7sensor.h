/**
 * MQ-7 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ7_SENSOR_PROCESS_H
#define _MQ7_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "mqsensor_process.h"

class MQ7SensorProcess: public MQSensorProcess {
	public:
		//@implement
		//@include "ecosense_cfg.h"
		MQ7SensorProcess(int pId, IProcessMessage* msg): MQSensorProcess(MQ7_ANALOG_PIN_ADDR, pId, msg) {
			TRACELNF("MQ7SensorProcess::init");
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("MQ7SensorProcess::factory");
			return new MQ7SensorProcess(pId, msg);
		}

		IProcessMessage* getResultMsg() {
			return new AirQualityMsg(CO, this->getQuality(.4), this->getVoltage());
		}
};

#endif
