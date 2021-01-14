/**
 * MQ-4 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ4_SENSOR_PROCESS_H
#define _MQ4_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "mqsensor_process.h"

//#include <Arduino.h>

#define MQ4_ANALOG_PIN 15
#define MQ4_DIGITAL_PIN 8

#define READINGS_PER_RESULT 50

#define MQ136_RL 2.3
#define MQ136_FRESH_RO 1

// curve params calculated using article: https://circuitdigest.com/microcontroller-projects/arduino-mq137-ammonia-sensor
const float CH4_CURVE[2] PROGMEM = {-0.2840, -0.9672};
const float H2_CURVE[2] PROGMEM = {-1.24, 0.7562};

class MQ4SensorProcess: public MQSensorProcess {
	public:
		MQ4SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		void update(unsigned long ms);

};

#endif
