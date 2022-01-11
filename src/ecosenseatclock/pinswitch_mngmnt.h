#ifndef _PINSWITCHMNGR_H
#define _PINSWITCHMNGR_H

    #include <Arduino.h>
    #include "processy_cfg.h"

    #define USEDPIN_NONE 255

    class PinSwitchManager {
        protected:
            PinSwitchManager(byte *pins, byte keysCnt) {
                this->keyPins = pins;
                this->pinsCnt = keysCnt;
                this->currentPin = USEDPIN_NONE;
                this->useStarted = 0;

                for (byte i = 0; i < this->pinsCnt; i++) {
                    pinMode(this->keyPins[i], OUTPUT);
                }

                this->closeAll();
            }

        public:
            /**
             * Request to use pin (start transaction)
             * 0 - fail, >0 - success (open time)
             */
            //@implement
            uint32_t requestPin(byte targetPin) {
                if (currentPin == USEDPIN_NONE) {
                    this->usePin(targetPin);
                    return this->useStarted;
                }
                return 0;
            }

            /**
             * Turn off previosly opened pin by key from requestPin, or forcibly off any pin
             * (force off: key = 0)
             * true - ok, false - nothing changed!
             */
            //@implement
            bool releasePin(uint32_t & key) {
                if (key == 0 || key == this->useStarted) {
                    this->closeAll();   // fully off
                    return true;
                }
                return false; // nothing changed!
            }

        protected:
            byte                *keyPins;  // arr
            byte                pinsCnt;   // cnt
            uint32_t            useStarted;
            byte                currentPin;

            //@implement
            void closeAll() {
                for (byte i = 0; i < this->pinsCnt; i++) {
                    byte pin = this->keyPins[i];
                    digitalWrite(pin, LOW);
                }
                this->useStarted = 0;
                this->currentPin = USEDPIN_NONE;
            }

            //@implement
            virtual void usePin(byte num) {
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
    };

#endif