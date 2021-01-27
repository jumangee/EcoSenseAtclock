#ifndef _ADCMUXMNGT_H
#define _ADCMUXMNGT_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "pinswitch_mngmnt.h"

    #define ADCMUX_SIGNAL_PIN 4

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

            static byte signalPin() {
                return ADCMUX_SIGNAL_PIN;
            }

        protected:
            void usePin(byte num);
    };

#endif