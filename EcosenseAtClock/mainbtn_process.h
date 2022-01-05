/**
 * EcoSense@clock Btn
 */

#ifndef _ECOSENSEBTN_PROCESS_H
#define _ECOSENSEBTN_PROCESS_H

#include "adcmuxbtn_process.h"

/**
 * @brief EcoSenseAtClock single control btn process
 * this btn proces reads at ADCMUX_SIGNAL_PIN if adc mux succesfully get access
 */
class EcoSenseAtClockBtnProcess: public ADCMuxButtonProcess {
	public:
		PROCESSID(PRC_BTN);

		EcoSenseAtClockBtnProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);
};

#endif