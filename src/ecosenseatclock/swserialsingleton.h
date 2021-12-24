#ifndef _SINGLETONCTRLR_H
#define _SINGLETONCTRLR_H

    #include <Arduino.h>
    #include <SoftwareSerial.h>
    
    class SoftwareSerialSingleton {
        protected:
            static SoftwareSerial* swSerial;

            //@cpp
            static SoftwareSerial* SoftwareSerialSingleton::swSerial = NULL;

        public:
            //@implement
            //@include <SoftwareSerial.h>
            static SoftwareSerial* get(byte rxpin, byte txpin, uint32_t baudrate) {
                if (SoftwareSerialSingleton::swSerial != NULL) {
                    return NULL;
                }

                SoftwareSerial* sws = new SoftwareSerial(rxpin, txpin);
                sws->begin(baudrate);
                SoftwareSerialSingleton::swSerial = sws;
                return sws;
            };

            //@implement
            //@include <SoftwareSerial.h>
            static SoftwareSerial* unlock() {
                delete SoftwareSerialSingleton::swSerial;
                SoftwareSerialSingleton::swSerial = NULL;
                return NULL;
            }

            //@implement
            //@include <SoftwareSerial.h>
            static bool isfree() {
                return SoftwareSerialSingleton::swSerial == NULL;
            }
    };
       
#endif