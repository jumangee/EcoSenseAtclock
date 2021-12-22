#include "mhz19sensor_process.h"
#include "SoftwareSerial.h"
#include "ecosense_messages.h"

        static uint8_t MHZ19SensorProcess::CMD_GETPPM[MHZ19_CMDSIZE] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
        static uint8_t MHZ19SensorProcess::CMD_SETRNG5000[MHZ19_CMDSIZE] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};
        static uint8_t MHZ19SensorProcess::CMD_AUTOCALOFF[MHZ19_CMDSIZE] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};         
        static uint8_t MHZ19SensorProcess::CMD_REBOOT[MHZ19_CMDSIZE] = {0xff, 0x01, 0x8d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73};
MHZ19SensorProcess::MHZ19SensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
            swSerial.begin(9600);
            sendCommand( MHZ19SensorProcess::CMD_AUTOCALOFF );
            sendCommand( MHZ19SensorProcess::CMD_SETRNG5000 );
	getData();    // первый запрос, в любом случае возвращает -1
            // pre-burn timeout
            this->pause(60000);    //mhz19
            TRACELNF("MHZ19SensorProcess pre-burn timeout")
}

static IFirmwareProcess* MHZ19SensorProcess::factory(IProcessMessage* msg) {
	return new MHZ19SensorProcess(msg);
}

void MHZ19SensorProcess::update(unsigned long ms) {
            getData();
            if (this->status != -1) {
                measureCount++;
                TRACEF("co2=")
                TRACELN(this->co2);
                if (measureCount > 2) {
                    this->report();
                    return;
                }
            }
            
	this->pause(12000);
}

void MHZ19SensorProcess::sendCommand(uint8_t cmd[], uint8_t* response = NULL) {
    swSerial.write(cmd, MHZ19_CMDSIZE);
    swSerial.flush();
    
    if (response != NULL) {
        int i = 0;
        while(swSerial.available() <= 0) {
            if( ++i > WAIT_READ_TIMES ) {
                TRACELNF("error: can't get MH-Z19 response.");
                return;
            }
            yield();
            delay(WAIT_READ_DELAY);
        }
        swSerial.readBytes(response, MHZ19_CMDSIZE);
    }
}

void MHZ19SensorProcess::getData() {
    uint8_t buf[MHZ19_CMDSIZE];
    for( int i=0; i<MHZ19_CMDSIZE; i++){
        buf[i]=0x0;
    }
    sendCommand(MHZ19SensorProcess::CMD_GETPPM, buf);
    byte crc = 0x00;
    for (byte i = 1; i < 8; i++) crc+=buf[i];
    crc = 0xff - crc + 0x01;
    
    if ( !(buf[0] == 0xFF && buf[1] == 0x86 && buf[8] == crc) ) {
        /*TRACEF("Error: 0=")
        TRACE(buf[0])
        TRACEF(", 1=")
        TRACE(buf[1])
        TRACEF(", crc=")
        TRACE(crc)
        TRACEF(" / ")
        TRACELN(buf[8]);*/
        this->status = -1;
        return;
    } 
    TRACEF("MHZ19: 0=")
    TRACE(buf[0])
    TRACEF(", 1=")
    TRACE(buf[1])
    TRACEF(", crc=")
    TRACE(crc)
    TRACEF(" / ")
    TRACELN(buf[8]);
    this->co2 = (256*((unsigned int) buf[2])) + ((unsigned int) buf[3]);
    this->temp = (buf[4]-32)*5/9;
    this->status = buf[5];
}	 
