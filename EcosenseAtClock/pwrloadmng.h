#ifndef _PWRLOADCNTRL_H
#define _PWRLOADCNTRL_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    
    //#define POWER_NONE 255
    #define USEDPIN_NONE 255

    class PinSwitchManager {
        protected:
            PinSwitchManager(byte pins[], byte keysCnt) {
                this->keyPins = pins;
                this->pinsCnt = keysCnt;
                this->currentPin = 0;
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
            uint32_t requestPin(byte targetPin);

            /**
             * Turn off previosly opened pin by key from requestPin, or forcibly off any pin
             * (force off: key = 0)
             * true - ok, false - nothing changed!
             */
            bool releasePin(uint32_t & key);

        protected:
            const byte          *keyPins;  // arr
            byte                pinsCnt;   // cnt
            uint32_t            useStarted;
            byte                currentPin;

            void closeAll();

            virtual void usePin(byte num);
    };

    class PowerloadManagement: public PinSwitchManager {
        protected:
            PowerloadManagement(byte pins[], byte keysCnt): PinSwitchManager(pins, keysCnt) {
                TRACELNF("PowerloadManagement")
            }

            static PowerloadManagement* instance;

        public:
            static PowerloadManagement* get() {
                return PowerloadManagement::instance;
            }

            static void init(const byte pins[], byte cnt) {
                if (!PowerloadManagement::instance) {
                    PowerloadManagement::instance = new PowerloadManagement(pins, cnt);
                }
            }
    };

    //in cpp
    //PowerloadManagement* PowerloadManagement::instance = NULL;

    class ADCMuxManagement: public PinSwitchManager {
        protected:
            ADCMuxManagement(byte pins[4]): PinSwitchManager(pins, 4) {
                TRACELNF("ADCMuxManagement")
            }

            static ADCMuxManagement* instance;
            const static byte channelTruthTable[16][4];

        public:
            static ADCMuxManagement* get() {
                return ADCMuxManagement::instance;
            }

            static void init(const byte pins[4]) {
                if (!ADCMuxManagement::instance) {
                    ADCMuxManagement::instance = new ADCMuxManagement(pins);
                }
            }

            void usePin(byte num);
    };


    /*class PowerloadManagement {
        private:
            PowerloadManagement(byte pins[], byte keysCnt) {
                TRACELNF("PowerloadManagement")
                this->keyPins = pins;
                this->keysCnt = keysCnt;
                this->openStarted = 0;
                this->releaseLoad(POWER_NONE);
            }

            static PowerloadManagement* instance;

        public:
            static PowerloadManagement* get() {
                return PowerloadManagement::instance;
            }

            static void init(const byte pins[], byte cnt) {
                if (!PowerloadManagement::instance) {
                    PowerloadManagement::instance = new PowerloadManagement(pins, cnt);
                }
                //return PowerloadManagement::instance;
            }
            
            / j**
             * Request to turn on specific load
             * 0 - fail, >0 - success (open time)
             *j/
            //!@implement
            uint32_t requestLoadSwitch(byte switchPin) {
                TRACEF("PowerloadManagement//requestLoadSwitch: ")
                TRACELN(switchPin)
                if (currentlyOpenKey == POWER_NONE) {
                    this->changeKey(switchPin);
                    return this->openStarted;
                } else if (currentlyOpenKey == switchPin) {
                    return this->openStarted;
                }
                return 0;
            }

            /j**
             * Turn off previosly opened load, or forcibly off any load
             * (force off: key = POWER_NONE)
             * true - ok, false - nothing changed!
             *j/
            //!@implement
            bool releaseLoad(byte key) {
                TRACEF("PowerloadManagement//release: ")
                TRACE(key)
                TRACEF(", current: ")
                TRACELN(this->currentlyOpenKey)
                if (key == POWER_NONE || key == this->currentlyOpenKey) {
                    this->changeKey(POWER_NONE);   // fully off
                    TRACELNF("PowerloadManagement//POWER_NONE")
                    return true;
                }
                return false; // nothing changed!
            }

        private:
            const byte          *keyPins;  // arr
            byte                keysCnt;   // cnt
            uint32_t            openStarted;
            byte                currentlyOpenKey;

            //!@implement
            void changeKey(byte num) {
                int numPos = -1;
                // fully off, then make
                for (byte i = 0; i < this->keysCnt; i++) {
                    byte pin = this->keyPins[i];
                    TRACEF("PowerloadManagement//pinOff: ")
                    TRACELN(pin)
                    digitalWrite(pin, LOW);
                    if (pin == num) {
                        numPos = i;
                    }
                }

                if (numPos > -1) {
                    digitalWrite(num, HIGH);
                    this->currentlyOpenKey = num;
                    this->openStarted = millis();
                    TRACEF("PowerloadManagement::POWERED pin=")
                    TRACELN(currentlyOpenKey)
                } else {
                    this->openStarted = 0;
                    this->currentlyOpenKey = POWER_NONE;
                }
            }
    };*/

#endif