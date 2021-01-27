/**
 * MQ-136 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ136_SENSOR_PROCESS_H
#define _MQ136_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
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
		MQ136SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		/*//@implement
		//!@include "ecosense_cfg.h"
		//!@include "mqsensor_process.h"
		void update(unsigned long ms) {
			if (!readingsDone(MQ136_ANALOG_PIN, READINGS_PER_RESULT)) {
				return;
			}

			{
				TRACEF("[ MQ-136 ] analog=");
				TRACE(this->getValue());
				//TRACEF(", dig=");
				//TRACE(this->mq136dig);
				TRACEF(", V=");
				TRACE( this->getVoltage() );
				TRACEF(", instant=");
				TRACE( this->instantValue(MQ136_ANALOG_PIN) );
				TRACEF(", Dig=");
				TRACELN( digitalRead(MQ136_DIGITAL_PIN) );
			}

			this->getHost()->sendMessage(new AirQualityMsg(H2S, this->getQuality(.6), this->getVoltage()));

			this->pause(ENVSENSORS_TIMEOUT);
		}*/

		IProcessMessage* getResultMsg() {
			return new AirQualityMsg(H2S, this->getQuality(.6), this->getVoltage());
		}

};

#endif
