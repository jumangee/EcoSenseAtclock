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
		//@include "forcedClimate/forcedClimate.cpp"
		BME280SensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			climateSensor.begin();
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new BME280SensorProcess(msg);
		}

		//@implement
		void update(unsigned long ms) {
			IProcessMessage* msg = readBME280();
			/*if (this->ready) {
				IProcessMessage* msg = readBME280();
				if (msg != NULL) {
					this->getHost()->sendMessage(msg);
				}
			} else {
				this->getHost()->sendMessage(new EnvDataMessage());
			}*/

			this->pause(8000);
		}

		//@implement
		//@include "forcedClimate/forcedClimate.cpp"
		//@include "ecosense_messages.h"
		IProcessMessage* readBME280() {
			climateSensor.takeForcedMeasurement();

			this->sendMessage(new EnvDataMessage(climateSensor.getTemperatureCelcius(), climateSensor.getRelativeHumidity(), climateSensor.getPressure() / 1.33));

			//EnvDataMessage* msg = new EnvDataMessage(climateSensor.getTemperatureCelcius(), climateSensor.getRelativeHumidity(), climateSensor.getPressure());

			/*TRACEF("TempC=")
			TRACE(climateSensor.getTemperatureCelcius())
			TRACEF(", hum=")
			TRACE(climateSensor.getRelativeHumidity())
			TRACEF(", press=")
			TRACELN(climateSensor.getPressure() / 1.33)*/

			return NULL;
		}
};

#endif