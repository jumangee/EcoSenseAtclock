#include "pinswitch_mngmnt.h"

uint32_t PinSwitchManager::requestPin(byte targetPin) {
    if (currentPin == USEDPIN_NONE) {
        this->usePin(targetPin);
        return this->useStarted;
    }
    return 0;
}

bool PinSwitchManager::releasePin(uint32_t & key) {
    if (key == 0 || key == this->useStarted) {
        this->closeAll();   // fully off
        return true;
    }
    return false; // nothing changed!
}

void PinSwitchManager::closeAll() {
    for (byte i = 0; i < this->pinsCnt; i++) {
        byte pin = this->keyPins[i];
        digitalWrite(pin, LOW);
    }
    this->useStarted = 0;
    this->currentPin = USEDPIN_NONE;
}

void PinSwitchManager::usePin(byte num) {
    // fully off, then make
    this->closeAll();
    for (byte i = 0; i < this->pinsCnt; i++) {
        byte pin = this->keyPins[i];
        if (num == pin) {
            digitalWrite(num, HIGH);
            this->currentPin = num;
            this->useStarted = millis();
        }
    }
}
