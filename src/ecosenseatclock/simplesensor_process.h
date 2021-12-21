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
		//@implement
		SimpleSensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			this->readingsCount = 0;
            this->value = 0;
		}

        uint16_t getValue() {
            return round(value / float(readingsCount));
        }

        virtual uint16_t getInstantValue() = 0;

        /**
         * Calc V from analog value
         */
        float getVoltage() {
            return (getValue() + .5) * (5.0 / 1023.0);
        }

		bool readingsDone(byte countPerResult) {
			if (readingsCount >= countPerResult) {
				this->readingsCount = 0;
				this->value = 0;
			}
	
			this->value += this->getInstantValue();
			this->readingsCount++;

			return (readingsCount >= countPerResult) ? true : false;
		}
};

#endif
