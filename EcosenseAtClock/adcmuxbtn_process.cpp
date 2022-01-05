#include "adcmuxbtn_process.h"

ADCMuxButtonProcess::ADCMuxButtonProcess(byte btnChannel, byte readPin, IProcessMessage* msg) : ADCMuxChannelProcess(msg), ButtonSensor(readPin){
            channel = btnChannel;
}

void ADCMuxButtonProcess::update(unsigned long ms) {
	if (getChannel(channel)) {
                this->sendMessage( ButtonSensor::update() );
                freeChannel();
	}
            this->pause(50);
}
