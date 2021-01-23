#include "pwrloadmng.h"

PowerloadManagement* PowerloadManagement::instance = NULL;

ADCMuxManagement* ADCMuxManagement::instance = NULL;

const static byte ADCMuxManagement::channelTruthTable[16][4] PROGMEM = {
    // s0, s1, s2, s3     channel
    {0,  0,  0,  0}, // 0
    {1,  0,  0,  0}, // 1
    {0,  1,  0,  0}, // 2
    {1,  1,  0,  0}, // 3
    {0,  0,  1,  0}, // 4
    {1,  0,  1,  0}, // 5
    {0,  1,  1,  0}, // 6
    {1,  1,  1,  0}, // 7
    {0,  0,  0,  1}, // 8
    {1,  0,  0,  1}, // 9
    {0,  1,  0,  1}, // 10
    {1,  1,  0,  1}, // 11
    {0,  0,  1,  1}, // 12
    {1,  0,  1,  1}, // 13
    {0,  1,  1,  1}, // 14
    {1,  1,  1,  1}  // 15
};

uint32_t PinSwitchManager::requestPin(byte targetPin) {
    //TRACEF("PinSwitchManager//requestPin: ")
    ///TRACELN(targetPin)
    if (currentPin == USEDPIN_NONE) {
        this->usePin(targetPin);
        return this->useStarted;
    } else if (currentPin == targetPin) {
        return this->useStarted;
    }
    return 0;
}

bool PinSwitchManager::releasePin(uint32_t & key) {
    /*TRACEF("PinSwitchManager//release: ")
    TRACE(key)
    TRACEF(", current: ")
    TRACELN(this->useStarted)*/
    if (key == 0 || key == this->useStarted) {
        this->closeAll();   // fully off
        //TRACELNF("PinSwitchManager//USEDPIN_NONE")
        return true;
    }
    return false; // nothing changed!
}

void PinSwitchManager::closeAll() {
    for (byte i = 0; i < this->pinsCnt; i++) {
        byte pin = this->keyPins[i];
        /*TRACEF("PinSwitchManager//pinOff: ")
        TRACELN(pin)*/
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
            TRACEF("PinSwitchManager//used pin=")
            TRACELN(currentPin)
        }
    }
}

void ADCMuxManagement::usePin(byte num) {
    if (num < 16) {
        TRACEF("ADCMuxChannel: ")
        TRACELN(num)
        for (byte i = 0; i < 4; i++) {
            TRACEF("ADCMux pin: ")
            TRACE(this->keyPins[i])
            TRACEF(" v=")
            TRACELN(ADCMuxManagement::channelTruthTable[num][i])
            digitalWrite(this->keyPins[i], ADCMuxManagement::channelTruthTable[num][i]); 
        }
        this->currentPin = num;
        this->useStarted = millis();
        TRACEF("ADCMuxChannel/DONE")
    }
}
