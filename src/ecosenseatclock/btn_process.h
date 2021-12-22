/**
 * Button sensor process
 * for Processy Firmware Framework
 */

#ifndef _BTNSENSOR_PROCESS_H
#define _BTNSENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"

class ButtonSensorProcess: public IFirmwareProcess {
	private:
        bool lastState = false;
        uint32_t pressDn = 0;
	public:
		PROCESSID(PRC_BTN);

		//@implement
		ButtonSensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new ButtonSensorProcess(msg);
		}

		//@implement
        //@include "adcmux_mngmnt.h"
        //@include "ecosense_messages.h"
		void update(unsigned long ms) {
            ADCMuxManagement* mux = ADCMuxManagement::get();
            uint32_t ticket = mux->requestChannel( MUXCHANNEL_BTN );
			if (ticket) {
				bool isPressed = ( ((float)analogRead( ADCMUX_SIGNAL_PIN )) * (5.0 / 1023.0) ) > 3.5;
				mux->release(ticket);
                uint32_t curTime = millis();
                if ( (curTime - pressDn) > 50 ) {
                    if (lastState == true && isPressed == false) {
                        // click done
                        pressDn = curTime;
                        this->sendMessage(new ButtonClickMessage());
                    } else if (lastState == false && isPressed == true) {
                        pressDn = curTime;
                    }
                }
                lastState = isPressed;
			}

            this->pause(50);
		}
};

#endif