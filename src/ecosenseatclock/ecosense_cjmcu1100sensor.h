/**
 * CJMCU-1100 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _CJMCU1100_SENSOR_PROCESS_H
#define _CJMCU1100_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "mqsensor_process.h"
#include "ecosense_cfg.h"

class CJMCU1100SensorProcess: public MQSensorProcess {
	public:
		//@implement
		//@include "ecosense_cfg.h"
		CJMCU1100SensorProcess(int pId, IProcessMessage* msg): MQSensorProcess(CJMCU1100_ANALOG_PIN_ADDR, pId, msg) {
			TRACELNF("CJMCU1100SensorProcess::init");
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("CJMCU1100SensorProcess::factory");
			return new CJMCU1100SensorProcess(pId, msg);
		}

		/*uint16_t getInstantValue() {
			return this(ADCMuxManagement::get()->getSignalPin());
		}

		//!@implement
		//!@include "ecosense_cfg.h"
		void update(unsigned long ms) {
			uint32_t ticket = ADCMuxManagement::get()->requestPin();
			if (ticket) {
				{
					bool done = this->readingsDone(READINGS_PER_RESULT);
					ADCMuxManagement::get()->releasePin(ticket);
					if (!done) {
						return;
					}
				}

				float v = this->getVoltage();

				// One parts per million is equal to 0.998859 milligrams per liter
				// 0.01mg/dm3 - critical, so, 10ppm is critial (10mg/m3)

				float logPPM_CH2O = (-2.631) + 1.528 * v + (-1.125) * pow(v, 2);

				this->getHost()->sendMessage(new AirQualityMsg(CH2O, byte(expf( logPPM_CH2O )), v));

				//this->getHost()->sendMessage(new AirQualityMsg(C6H5_CH3, this->getQuality(.4), v));
			}

			this->pause(ENVSENSORS_TIMEOUT);
		}*/

		IProcessMessage* getResultMsg() {
			float v = this->getVoltage();

			// One parts per million is equal to 0.998859 milligrams per liter
			// 0.01mg/dm3 - critical, so, 10ppm is critial (10mg/m3)

			float logPPM_CH2O = (-2.631) + 1.528 * v + (-1.125) * pow(v, 2);

			return new AirQualityMsg(CH2O, byte(expf( logPPM_CH2O )), v));
		}

};

#endif
