/**
 * MHZ19 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _MHZ19_SENSOR_PROCESS_H
#define _MHZ19_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "ecosense_cfg.h"
#include "simple_sensor_process.h"

#include "MHZ19_uart/MHZ19_uart.h"

class MHZ19SensorProcess: public SimpleSensorProcess {
		MHZ19_uart		mhz19;
		bool			mhz19active;
        uint32_t        startTime;

	public:
		MHZ19SensorProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

        uint16_t getInstantValue() {
            return mhz19.getPPM();
        }

		void update(unsigned long ms);

};

#endif
