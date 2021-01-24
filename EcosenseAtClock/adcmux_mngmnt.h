#ifndef _ADCMUXMNGT_H
#define _ADCMUXMNGT_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "pinswitch_mngmnt.h"

    class ADCMuxManagement: public PinSwitchManager {
        protected:
            ADCMuxManagement(byte *pins): PinSwitchManager(pins, 4) {
            }

            static ADCMuxManagement* instance;
            const static byte channelTruthTable[16][4];

        public:
            static ADCMuxManagement* get() {
                return ADCMuxManagement::instance;
            }

            //static void init(const byte pins[4]) {
            static void init(const byte (&pins)[4]) {
                if (!ADCMuxManagement::instance) {
                    ADCMuxManagement::instance = new ADCMuxManagement(pins);
                }
            }

        protected:
            void usePin(byte num);
    };

#endif