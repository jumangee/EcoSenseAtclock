#include "mainbtn_process.h"

EcoSenseAtClockBtnProcess::EcoSenseAtClockBtnProcess(IProcessMessage* msg) : ADCMuxButtonProcess(MUXCHANNEL_BTN, ADCMUX_SIGNAL_PIN, msg){
}

static IFirmwareProcess* EcoSenseAtClockBtnProcess::factory(IProcessMessage* msg) {
	return new EcoSenseAtClockBtnProcess(msg);
}
