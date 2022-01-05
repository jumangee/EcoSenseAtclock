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

		ButtonClickMessage* update();

        ButtonClickMessage* getBtnEvent(BUTTON_EVENT type);
};

#endif