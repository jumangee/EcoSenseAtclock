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

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("PPD42SensorProcess::factory");
			return new PPD42SensorProcess(pId, msg);
		}

        bool readingsDone2(byte countPerResult) {
			if (readingsCount2 >= countPerResult) {
				readingsCount2 = 0;
				value2 = 0;
			}
	
			this->value2 += this->getInstantValue();
			readingsCount2++;

			if (readingsCount2 >= countPerResult) {
				value2 = round(value2 / float(readingsCount2));
				return true;
			}
			return false;
		}

		//@implement
		//@include "ecosense_cfg.h"
		void update(unsigned long ms) {
            uint32_t ticket;
            ADCMuxManagement* adc = ADCMuxManagement::get();

            ticket = adc->requestPin(PPD42NS_ANALOG_PIN_ADDR1);	// ADC MUX PIN!
			if (ticket) {
				{
					bool done = this->readingsDone(READINGS_PER_RESULT);
					adc->releasePin(ticket);
					if (!done) {
						return;
					}
				}

				this->getHost()->sendMessage(new AirQualityMsg(PM1, 0, this->getVoltage()));
			}

            ticket = adc->requestPin(PPD42NS_ANALOG_PIN_ADDR2);             // ADC MUX PIN!
			if (ticket) {
				{
					bool done = this->readingsDone2(READINGS_PER_RESULT);
					adc->releasePin(ticket);
					if (!done) {
						return;
					}
				}

				this->getHost()->sendMessage(new AirQualityMsg(PM25, 0, this->getVoltage()));
			}
            
			this->pause(ENVSENSORS_TIMEOUT);
		}

};

#endif
