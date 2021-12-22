#include "btn_process.h"
#include "adcmux_mngmnt.h"
#include "ecosense_messages.h"

ButtonSensorProcess::ButtonSensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
}

static IFirmwareProcess* ButtonSensorProcess::factory(IProcessMessage* msg) {
	return new ButtonSensorProcess(msg);
}

void ButtonSensorProcess::update(unsigned long ms) {
            ADCMuxManagement* mux = ADCMuxManagement::get();
            uint32_t ticket = mux->requestChannel( MUXCHANNEL_BTN );
	if (ticket) {
		bool isPressed = ( ((float)analogRead( ADCMUX_SIGNAL_PIN )) * (5.0 / 1023.0) ) > 3.5;
		mux->release(ticket);
                uint32_t curTime = millis();
                if (lastState == true && isPressed == false && (curTime - pressDn) > 100) {
                    // click done
                    //TRACELNF("Click!")
                    pressDn = 0;
                    this->sendMessage(new ButtonClickMessage());
                } else if (lastState == false && isPressed == true) {
                    //TRACELNF("Keydown")
                    pressDn = curTime;
                }
                lastState = isPressed;
	}
            this->pause(50);
}
