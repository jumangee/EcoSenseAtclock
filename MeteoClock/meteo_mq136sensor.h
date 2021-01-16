/**
 * MQ-136 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ136_SENSOR_PROCESS_H
#define _MQ136_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "mqsensor_process.h"

//#include <Arduino.h>

#define MQ136_ANALOG_PIN A1
//16
#define MQ136_DIGITAL_PIN 7

#define READINGS_PER_RESULT 50

#define MQ136_RL 2.3
#define MQ136_FRESH_RO 1

// curve params calculated using article: https://circuitdigest.com/microcontroller-projects/arduino-mq137-ammonia-sensor
//const float H2S_CURVE[2] PROGMEM = {-1.51, 1.088};		// m: [log(ppm1 / "RsRo1") - log(ppm2 / "RsRo2")], b: [log("RsRo3") - m * log(ppm3)]
//const float CO_CURVE[2] PROGMEM = {-1.24, 0.7562};		// m: [log(ppm1 / "RsRo1") - log(ppm2 / "RsRo2")], b: [log("RsRo3") - m * log(ppm3)]

//const float H2S_CURVE[2] PROGMEM = {40.44109566, -1.085728557};
//const float CO_CURVE[2] PROGMEM = {2142.297846, -2.751369226};

class MQ136SensorProcess: public MQSensorProcess {
	public:
		MQ136SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		void update(unsigned long ms);

};

#endif
