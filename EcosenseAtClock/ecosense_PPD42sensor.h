/**
 * PPD42 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _PPD42_SENSOR_PROCESS_H
#define _PPD42_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "ecosense_cfg.h"

#include "simple_sensor_process.h"

class PPD42SensorProcess: public SimpleSensorProcess {
	private:
		byte readingsCount2;
        uint16_t value2;

	public:
		PPD42SensorProcess(int pId, IProcessMessage* msg): SimpleSensorProcess(pId, msg) {
            readingsCount2 = 0;
            value2 = 0;
			TRACELNF("PPD42SensorProcess::init");
		}

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		uint16_t getInstantValue();

        bool readingsDone2(byte countPerResult);

		void update(unsigned long ms);

};

#endif
