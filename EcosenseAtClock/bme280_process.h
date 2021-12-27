/**
 * BME280 environment sensor process
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_SENSORS_H
#define _ECOSENSEATCLOCK_SENSORS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"

#include "forcedClimate/forcedClimate.h"

class BME280SensorProcess: public IFirmwareProcess {
	private:
		ForcedClimate climateSensor;

	public:
		PROCESSID(PRC_BME280);

		BME280SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);
};

#endif
