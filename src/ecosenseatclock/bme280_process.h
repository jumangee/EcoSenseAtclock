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

		//@implement
		BME280SensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			climateSensor.begin();
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new BME280SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		//@include "forcedClimate/forcedClimate.cpp"
		void update(unsigned long ms) {
			climateSensor.takeForcedMeasurement();
			this->sendMessage(new EnvDataMessage(climateSensor.getTemperatureCelcius(), climateSensor.getRelativeHumidity(), climateSensor.getPressure() / 1.33));

			this->pause(8000);
		}
};

#endif
