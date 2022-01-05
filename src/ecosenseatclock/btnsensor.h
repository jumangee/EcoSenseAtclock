/**
 * Button sensor BASE
 */

#ifndef _BTNSENSOR_BASE_H
#define _BTNSENSOR_BASE_H

#include <Arduino.h>
#include "ecosense_messages.h"

#define BTNCLICK_TIME 300
#define BTNHOLD_TIME 5000

class ButtonSensor {
	private:
        bool        lastBtnState = false;
        uint32_t    pressDn = 0;
        uint32_t    lastClick = 0;
        byte        btnPin;
	public:
        ButtonSensor(byte pin) {
            this->btnPin = pin;
		}

        //@implement
        //@include "ecosense_messages.h"
		ButtonClickMessage* update() {
            bool isPressed = ( ((float)analogRead( btnPin )) * (5.0 / 1023.0) ) > 3.5;
            uint32_t curTime = millis();
            if (lastBtnState == true && isPressed == false) {
                uint32_t pressed = (curTime - pressDn);
                #if BTN_SUPPORT_HOLD == 1
                if ( pressed > BTNHOLD_TIME ) {
                    // hold
                    return this->getBtnEvent( BUTTON_EVENT::HOLD );
                } else {
                #endif
                    #if BTN_SUPPORT_DBLCLCK == 1
                    // click
                    if (lastClick > 0 && (curTime - lastClick < BTNCLICK_TIME)) { // DOUBLE CLICK
                        lastClick = 0;
                        return this->getBtnEvent( BUTTON_EVENT::DBLCLICK );
                    } else {
                    #endif
                        // remember this tap!)
                        lastClick = curTime;
                    #if BTN_SUPPORT_DBLCLCK == 1
                    }
                    #endif
                #if BTN_SUPPORT_HOLD == 1
                }
                #endif
            } else if (lastBtnState == false) {
                if (isPressed == true) {
                    pressDn = curTime;
                } else if (lastClick > 0 && (millis() - lastClick < BTNCLICK_TIME)) {
                    // react on single click
                    return this->getBtnEvent( BUTTON_EVENT::CLICK );
                }
            }
            return NULL;
		}

        //@implement
        //@include "ecosense_messages.h"
        ButtonClickMessage* getBtnEvent(BUTTON_EVENT type) {
            lastClick = 0;
            pressDn = 0;
            lastBtnState = false;

            return new ButtonClickMessage( type );
        }
};

#endif