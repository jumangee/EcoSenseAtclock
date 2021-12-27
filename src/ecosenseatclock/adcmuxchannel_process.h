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

        //@implement
		//@include "adcmux_mngmnt.h"
		bool getChannel(byte ch) {
			ticket = ADCMuxManagement::get()->requestChannel(ch);
			return ticket > 0;
		}

        //@implement
		//@include "adcmux_mngmnt.h"
        void freeChannel() {
            ADCMuxManagement::get()->release(ticket);
        }
};

#endif
