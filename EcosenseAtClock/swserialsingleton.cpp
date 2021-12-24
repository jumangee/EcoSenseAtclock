#include "swserialsingleton.h"
#include <SoftwareSerial.h>

            static SoftwareSerial* SoftwareSerialSingleton::swSerial = NULL;
static SoftwareSerial* SoftwareSerialSingleton::get(byte rxpin, byte txpin, uint32_t baudrate) {
    if (SoftwareSerialSingleton::swSerial != NULL) {
        return NULL;
    }
    SoftwareSerial* sws = new SoftwareSerial(rxpin, txpin);
    sws->begin(baudrate);
    SoftwareSerialSingleton::swSerial = sws;
    return sws;
};

static SoftwareSerial* SoftwareSerialSingleton::unlock() {
    delete SoftwareSerialSingleton::swSerial;
    SoftwareSerialSingleton::swSerial = NULL;
    return NULL;
}

static bool SoftwareSerialSingleton::isfree() {
    return SoftwareSerialSingleton::swSerial == NULL;
}
