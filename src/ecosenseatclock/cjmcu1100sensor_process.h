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
		
		//@implement
		//@include "ecosense_cfg.h"
		CJMCU1100SensorProcess(IProcessMessage* msg): MQSensorProcess(MUXCHANNEL_CJMCU1100, msg) {
            this->pause(30000);    // pre-burn timeout
			TRACELNF("CJMCU1100SensorProcess: pre-burn timeout")
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new CJMCU1100SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		IProcessMessage* getResultMsg() {
			float v = this->getVoltage();

			float ppm = max(
				exp( (-2.631) + 1.528 * v + (-1.125) * pow(v, 2) ),		// formaldehyde	// 0.01mg/dm3 - critical, so, 10ppm is critial (10mg/m3)
				exp( (-7.071) + 2.852 * v + (-0.210) * pow(v, 2) )		// toluene
			);

			return new AirQualityMsg(AirQualityMsg::GasType::VOCs, AirQualityMsg::value2code(round(min(ppm, 10.0) / 2)), round(ppm));
		}

};

#endif
