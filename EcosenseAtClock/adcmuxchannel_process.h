/**
 * Base adxmux-sensor process
 * for Processy Firmware Framework
 */

#ifndef _ADCMUX_SENSOR_PROCESS_H
#define _ADCMUX_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

class ADCMuxChannelProcess: public IFirmwareProcess {
	private:
        uint32_t ticket = 0;

	public:
		ADCMuxChannelProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
		}

		bool getChannel(byte ch);

        void freeChannel();
};

#endif
