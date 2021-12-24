#include "mhz19sensor_process.h"
#include "SoftwareSerial.h"
#include "ecosense_messages.h"
#include "swserialsingleton.h"

        static uint8_t MHZ19SensorProcess::CMD_GETPPM[MHZ19_CMDSIZE] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
        static uint8_t MHZ19SensorProcess::CMD_SETRNG5000[MHZ19_CMDSIZE] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};
        static uint8_t MHZ19SensorProcess::CMD_AUTOCALOFF[MHZ19_CMDSIZE] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};         
MHZ19SensorProcess::MHZ19SensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
            this->pause(MHZ19_PREBURN_TIMEOUT);
}

static IFirmwareProcess* MHZ19SensorProcess::factory(IProcessMessage* msg) {
	return new MHZ19SensorProcess(msg);
}

void MHZ19SensorProcess::update(unsigned long ms) {
            if (swSerial == NULL) {
                TRACELN("MHZ19: trying to connect")
                swSerial = SoftwareSerialSingleton::get(MHZ19_RXPIN, MHZ19_TXPIN, 9600);
                if (swSerial != NULL) {
                    TRACELN("MHZ19: GOT! ticket")
                    sendCommand( MHZ19SensorProcess::CMD_AUTOCALOFF );
                    sendCommand( MHZ19SensorProcess::CMD_SETRNG5000 );
                    getData();    // first request always fails
                } else {
                    TRACELN("MHZ19: no ticket")
                }
                this->pause(500);
                return;
            } else {
                getData();
                swSerial = SoftwareSerialSingleton::unlock();
                this->sendMessage(new AirQualityMsg(AirQualityMsg::GasType::CO2, 
                    this->co2 < 600 ? AirQualityMsg::GasConcentration::MINIMAL : (
                        this->co2 > 2500 ? AirQualityMsg::GasConcentration::DANGER : (
                            this->co2 > 1000 ? AirQualityMsg::GasConcentration::WARNING : AirQualityMsg::GasConcentration::NORM
                        )
                    ),
                    (float)this->co2)
                );
                //this->sendMessage(new TaskDoneMessage(this));
                //this->pause();
                this->pause(CONSUMERPROCESSTIMEOUT);
                return;
            }
}

void MHZ19SensorProcess::sendCommand(uint8_t cmd[], uint8_t* response = NULL) {
    swSerial->write(cmd, MHZ19_CMDSIZE);
    swSerial->flush();
    
    if (response != NULL) {
        int i = 0;
        while(swSerial->available() <= 0) {
            if( ++i > WAIT_READ_TIMES ) {
                TRACELNF("error: can't get MH-Z19 response.");
                return;
            }
            yield();
            delay(WAIT_READ_DELAY);
        }
        swSerial->readBytes(response, MHZ19_CMDSIZE);
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
        this->status = -1;
        return;
    } 
    this->co2 = (256*( (uint16_t)buf[2] )) + ( (uint16_t)buf[3] );
    //this->temp = buf[4]-40;
    this->status = buf[5];
}	 
