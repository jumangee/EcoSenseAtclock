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
		ZE08CH2OSensorProcess(int pId, IProcessMessage* msg): MQSensorProcess(ZE08CH20_ANALOG_PIN_ADDR, pId, msg) {
			TRACELNF("ZE08CH2OSensorProcess::init");
		}

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		IProcessMessage* getResultMsg() {
			float v = this->getVoltage() - 0.4;
			return new AirQualityMsg(CH2O, 127 * (v / 1.6), v);	// DAC (0.4 ~ 2V, for 0-5ppm)
		}

};

#endif
