/**
 * Base sensor process
 * for Processy Firmware Framework
 */

#ifndef _SIMPLE_SENSOR_PROCESS_H
#define _SIMPLE_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

class SimpleSensorProcess: public IFirmwareProcess {
	private:
		byte readingsCount;
        uint16_t value;

	public:
		SimpleSensorProcess(uint16_t pId, IProcessMessage* msg);

        /*uint16_t getInstantValue(byte pin) {
            return analogRead(pin);
        }*/

        uint16_t getValue() {
            return value;
        }

        virtual uint16_t getInstantValue() = 0;

        /**
         * Calc V from analog value
         */
        float getVoltage() {
            return (value + .5) * (5.0 / 1023.0);
        }

		bool readingsDone(byte countPerResult) {
			if (readingsCount >= countPerResult) {
				readingsCount = 0;
				value = 0;
			}
	
			this->value += this->getInstantValue();
			readingsCount++;

			if (readingsCount >= countPerResult) {
				value = round(value / float(readingsCount));
				return true;
			}
			return false;
		}
};

#endif
