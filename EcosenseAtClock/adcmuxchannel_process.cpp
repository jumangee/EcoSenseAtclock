#include "adcmuxchannel_process.h"
#include "adcmux_mngmnt.h"

bool ADCMuxChannelProcess::getChannel(byte ch) {
	ticket = ADCMuxManagement::get()->requestChannel(ch);
	return ticket > 0;
}

void ADCMuxChannelProcess::freeChannel() {
    ADCMuxManagement::get()->release(ticket);
}
