#ifndef _ADCMUXMNGT_H
#define _ADCMUXMNGT_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "pinswitch_mngmnt.h"

    class ADCMuxManagement: protected PinSwitchManager {
        protected:
			byte channelPin;
			
            ADCMuxManagement(byte channelPin, byte *pins): PinSwitchManager(pins, 4) {
            }

            static ADCMuxManagement* instance;
            const static byte channelTruthTable[16][4];

        public:
            static ADCMuxManagement* get() {
                return ADCMuxManagement::instance;
            }

            static void init(byte channelPin, const byte (&pins)[4]) {
                if (!ADCMuxManagement::instance) {
                    ADCMuxManagement::instance = new ADCMuxManagement(channelPin, pins);
                }
            }

            static byte signalPin() {
                return ADCMuxManagement::get()->channelPin;
            }
			
			uint32_t requestPin() {
				return PinSwitchManager::requestPin(this->channelPin);
			}

            bool release(uint32_t & key) {
                return PinSwitchManager::releasePin(key);
            }

        protected:
            void usePin(byte num);
    };

#endif