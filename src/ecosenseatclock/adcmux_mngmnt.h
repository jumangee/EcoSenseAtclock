#ifndef _ADCMUXMNGT_H
#define _ADCMUXMNGT_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "pinswitch_mngmnt.h"

    class ADCMuxManagement: protected PinSwitchManager {
        protected:
            ADCMuxManagement(byte *pins): PinSwitchManager(pins, 4) {
            }

            static ADCMuxManagement* instance;
            const static byte channelTruthTable[16][4];

        public:
            static ADCMuxManagement* get() {
                return ADCMuxManagement::instance;
            }

            static void init(const byte (&pins)[4]) {
                if (!ADCMuxManagement::instance) {
                    ADCMuxManagement::instance = new ADCMuxManagement(pins);
                }
            }

            /*static byte signalPin() {
                return ADCMuxManagement::get()->signalPin;
            }*/
			
			uint32_t requestChannel(byte channel) {
				return PinSwitchManager::requestPin(channel);
			}

            bool release(uint32_t & key) {
                return PinSwitchManager::releasePin(key);
            }

        protected:
            //@implement
            void usePin(byte num) {
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
    };

#endif