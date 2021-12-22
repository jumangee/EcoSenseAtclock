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

//#define MQ136_ANALOG_PIN 22	// READ-ONLY PIN!
//#define MQ136_DIGITAL_PIN 7

//#define READINGS_PER_RESULT 50

//#define MQ136_RL 2.3
//#define MQ136_FRESH_RO 1

// curve params calculated using article: https://circuitdigest.com/microcontroller-projects/arduino-mq137-ammonia-sensor
//const float H2S_CURVE[2] PROGMEM = {-1.51, 1.088};		// m: [log(ppm1 / "RsRo1") - log(ppm2 / "RsRo2")], b: [log("RsRo3") - m * log(ppm3)]
//const float CO_CURVE[2] PROGMEM = {-1.24, 0.7562};		// m: [log(ppm1 / "RsRo1") - log(ppm2 / "RsRo2")], b: [log("RsRo3") - m * log(ppm3)]

//const float H2S_CURVE[2] PROGMEM = {40.44109566, -1.085728557};
//const float CO_CURVE[2] PROGMEM = {2142.297846, -2.751369226};

class MQ136SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ136);
		
		//@implement
		MQ136SensorProcess(IProcessMessage* msg): MQSensorProcess(MUXCHANNEL_MQ136, msg) {
            this->pause(30000);    // pre-burn timeout
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MQ136SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		IProcessMessage* getResultMsg() {
			float res = this->getVoltage();
			return new AirQualityMsg(
				AirQualityMsg::GasType::H2S, 
				res < 1.5 ? AirQualityMsg::GasConcentration::MINIMAL : (
					res < 3 ? AirQualityMsg::GasConcentration::NORM : (
						res < 4 ? AirQualityMsg::GasConcentration::WARNING : AirQualityMsg::GasConcentration::DANGER
					)
				),
				res
			);
		}

};

#endif
