#ifndef _SINGLETONCTRLR_H
#define _SINGLETONCTRLR_H

    #include <Arduino.h>
    #include <SoftwareSerial.h>
    
    class SoftwareSerialSingleton {
        protected:
            static SoftwareSerial* swSerial;

        public:
            static SoftwareSerial* get(byte rx, byte tx) {
                if (SignletonController::swSerial != NULL) {
                    return NULL;
                }

                SignletonController::swSerial = new SoftwareSerial(rx, tx);
                return SignletonController::swSerial;
            };

            static bool unlock() {
                delete SignletonController::swSerial;
                SignletonController::swSerial = NULL;
            }
    };
       
#endif