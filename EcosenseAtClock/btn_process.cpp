#include "btn_process.h"
#include "ecosense_messages.h"

ButtonSensorProcess::ButtonSensorProcess(IProcessMessage* msg) : ADCMuxChannelProcess(msg){
}

static IFirmwareProcess* ButtonSensorProcess::factory(IProcessMessage* msg) {
	return new ButtonSensorProcess(msg);
}

void ButtonSensorProcess::update(unsigned long ms) {
            //ADCMuxManagement* mux = ADCMuxManagement::get();
            //uint32_t ticket = mux->requestChannel( MUXCHANNEL_BTN );
	if (getChannel(MUXCHANNEL_BTN)) {
		bool isPressed = ( ((float)analogRead( ADCMUX_SIGNAL_PIN )) * (5.0 / 1023.0) ) > 3.5;
		//mux->release(ticket);
                freeChannel();
                uint32_t curTime = millis();
                uint32_t pressed = (curTime - pressDn);
                if ( pressed > 50 ) {
                    if (lastState == true && isPressed == false) {
                        // click done
                        pressDn = curTime;
                        this->sendMessage(new ButtonClickMessage( pressed > 5000 ? ButtonClickMessage::ButtonEvent::HOLD : ButtonClickMessage::ButtonEvent::CLICK ));
                    } else if (lastState == false && isPressed == true) {
                        pressDn = curTime;
                    }
                }
                lastState = isPressed;
	}
            this->pause(50);
}
