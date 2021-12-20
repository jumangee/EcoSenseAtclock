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
		SimpleSensorProcess(IProcessMessage* msg);

        uint16_t getValue() {
            return this->value;
        }

        virtual uint16_t getInstantValue() = 0;

        /**
         * Calc V from analog value
         */
        float getVoltage() {
            return (this->value + .5) * (5.0 / 1023.0);
        }

		bool readingsDone(byte countPerResult) {
			if (readingsCount >= countPerResult) {
				this->readingsCount = 0;
				this->value = 0;
			}
	
			this->value += this->getInstantValue();
			this->readingsCount++;

			if (readingsCount >= countPerResult) {
				this->value = round(value / float(readingsCount));
				return true;
			}
			return false;
		}
};

#endif
