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
		ADCMuxButtonProcess(byte btnChannel, byte readPin, IProcessMessage* msg);

		void update(unsigned long ms);
};

#endif