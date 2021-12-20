/**
 * ME2-CH20 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _ZE08CH20_SENSOR_PROCESS_H
#define _ZE08CH20_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "ecosense_cfg.h"

#include "mqsensor_process.h"

class ZE08CH2OSensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_ZE08CH20);
		
		ZE08CH2OSensorProcess(IProcessMessage* msg): MQSensorProcess(msg) {
			TRACELNF("ZE08CH2OSensorProcess::init");
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new ZE08CH2OSensorProcess(msg);
		}

		IProcessMessage* getResultMsg() {
			float v = this->getVoltage() - 0.4;
			return new AirQualityMsg(CH2O, 127 * (v / 1.6), v);	// DAC (0.4 ~ 2V, for 0-5ppm)
		}

};

#endif
