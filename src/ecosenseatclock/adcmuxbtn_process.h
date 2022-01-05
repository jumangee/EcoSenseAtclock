/**
 * Button sensor process
 * for Processy Firmware Framework
 */

#ifndef _BTNSENSOR_PROCESS_H
#define _BTNSENSOR_PROCESS_H

#include <Arduino.h>
#include "ecosense_cfg.h"
#include "adcmuxchannel_process.h"
#include "btnsensor.h"

class ADCMuxButtonProcess: public ADCMuxChannelProcess, public ButtonSensor {
    protected:
        byte channel;

	public:
		//@implement
		ADCMuxButtonProcess(byte btnChannel, byte readPin, IProcessMessage* msg): ADCMuxChannelProcess(msg), ButtonSensor(readPin) {
            channel = btnChannel;
		}

		//@implement
		void update(unsigned long ms) {
			if (getChannel(channel)) {
                this->sendMessage( ButtonSensor::update() );
                freeChannel();
			}
            this->pause(50);
		}
};

#endif