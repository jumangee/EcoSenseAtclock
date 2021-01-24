#include "adcmux_mngmnt.h"

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

void ADCMuxManagement::usePin(byte num) {
    if (num < 16) {
        /*TRACEF("ADCMuxChannel: ")
        TRACELN(num)*/
        for (byte i = 0; i < 4; i++) {
            //byte v = pgm_read_byte(&(ADCMuxManagement::channelTruthTable[num][i]));
            /*TRACEF("ADCMux pin: ")
            TRACE(this->keyPins[i])
            TRACEF(" v=")
            TRACELN(v)*/
            digitalWrite(this->keyPins[i], pgm_read_byte(&(ADCMuxManagement::channelTruthTable[num][i]))); 
        }
        this->currentPin = num;
        this->useStarted = millis();
        /*TRACE("ADCMuxChannel/DONE: ")
        TRACELN(this->currentPin)*/
    }
}
