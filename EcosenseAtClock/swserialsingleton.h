#ifndef _SINGLETONCTRLR_H
#define _SINGLETONCTRLR_H

    #include <Arduino.h>
    #include <SoftwareSerial.h>
    
    class SoftwareSerialSingleton {
        protected:
            static SoftwareSerial* swSerial;


        public:
            static SoftwareSerial* get(byte rxpin, byte txpin, uint32_t baudrate);

            static SoftwareSerial* unlock();

            static bool isfree();
    };
       
#endif