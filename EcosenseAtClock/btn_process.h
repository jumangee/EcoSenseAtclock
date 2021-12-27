/**
 * Button sensor process
 * for Processy Firmware Framework
 */

#ifndef _BTNSENSOR_PROCESS_H
#define _BTNSENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"

#include "adcmuxchannel_process.h"

class ButtonSensorProcess: public ADCMuxChannelProcess {
	private:
        bool lastState = false;
        uint32_t pressDn = 0;
	public:
		PROCESSID(PRC_BTN);

		ButtonSensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);
};

#endif