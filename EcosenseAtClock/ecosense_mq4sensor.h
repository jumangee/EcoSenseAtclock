/**
 * MQ-4 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ4_SENSOR_PROCESS_H
#define _MQ4_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "mqsensor_process.h"

#define MQ4_ANALOG_PIN 19	// READ-ONLY PIN!
#define MQ4_DIGITAL_PIN 8

#define READINGS_PER_RESULT 50

#define MQ136_RL 2.3
#define MQ136_FRESH_RO 1

class MQ4SensorProcess: public MQSensorProcess {
	public:
		MQ4SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		void update(unsigned long ms);

};

#endif
