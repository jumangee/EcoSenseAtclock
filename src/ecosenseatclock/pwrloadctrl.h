#ifndef _PWRLOADCNTRL_H
#define _PWRLOADCNTRL_H

    #include <Arduino.h>

    #define POWER_NONE 255

    class PowerloadManagement {
        private:
            PowerloadManagement(byte *pins, byte keysCnt) {
                this->keyPins = pins;
                this->keysCnt = keysCnt;
                this->openStarted = 0;
                this->release();
            }

            static PowerloadManagement* instance;

        public:
            static PowerloadManagement get() {
                return PowerloadManagement::instance;
            }

            static PowerloadManagement create(byte *pins, byte cnt) {
                if (!PowerloadManagement::instance) {
                    PowerloadManagement::instance = new PowerloadManagement(pins, cnt);
                }
                return PowerloadManagement::instance;
            }
            
            uint32_t requestLoadSwitch(byte c) {
                if (currentlyOpenKey == POWER_NONE) {
                    this->setConsumer(c);
                    return this->openStarted;
                } else if (currentlyOpenKey == c) {
                    return this->openStarted;
                }
                return 0;
            }

            bool releaseLoad(byte key) {
                if (key == POWER_NONE || key == this->currentlyOpenKey) {
                    this->changeKey();   // fully off
                    return true;
                }
                return false; // nothing changed!
            }

        private:
            byte                *keyPins;  // arr
            byte                keysCnt;   // cnt
            uint32_t            openStarted;
            byte                currentlyOpenKey;

            void changeKey(byte num, byte value) {
                // fully off, then make
                for (byte i = 0; i < this->keysCnt; i++) {
                    digitalWrite(this[keyPins], LOW);
                }
                this->openStarted = 0;
                this->currentlyOpenKey = POWER_NONE;

                if (num <= this->keysCnt) {
                    digitalWrite(this->keyPins[num], HIGH);
                    this->currentlyOpenKey = this->keyPins[num];
                    this->openStarted = millis();
                }
            }
    }

#endif