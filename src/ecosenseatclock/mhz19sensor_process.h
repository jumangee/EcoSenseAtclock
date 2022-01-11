/**
 * MHZ19 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _MHZ19_SENSOR_PROCESS_H
#define _MHZ19_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "ecosense_messages.h"

#define MHZ19_CMDSIZE 9

#include "SoftwareSerial.h"

class MHZ19SensorProcess: public IFirmwareProcess {
    protected:
        SoftwareSerial  *swSerial = NULL;

        int     co2 = 0;
        //int     temp = 0;
        int     status = -1;
        bool    ready;

        static uint8_t CMD_GETPPM[MHZ19_CMDSIZE];
        static uint8_t CMD_SETRNG5000[MHZ19_CMDSIZE];
        static uint8_t CMD_AUTOCALOFF[MHZ19_CMDSIZE];
        //static uint8_t CMD_RESET[MHZ19_CMDSIZE];

        //@cpp
        static uint8_t MHZ19SensorProcess::CMD_GETPPM[MHZ19_CMDSIZE] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
        //@cpp
        static uint8_t MHZ19SensorProcess::CMD_SETRNG5000[MHZ19_CMDSIZE] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};
        //@cpp
        static uint8_t MHZ19SensorProcess::CMD_AUTOCALOFF[MHZ19_CMDSIZE] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};         
        /*//@cpp
        static uint8_t MHZ19SensorProcess::CMD_RESET[MHZ19_CMDSIZE] = {0xff, 0x01, 0x8d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71};*/

	public:
        PROCESSID(PRC_MHZ19);

		//@implement
        //@include "SoftwareSerial.h"
		MHZ19SensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
            this->pause(MHZ19_PREBURN_TIMEOUT);
			TRACELNF("MHZ19: pre-burn timeout")
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MHZ19SensorProcess(msg);
		}

		//@implement
        //@include "ecosense_messages.h"
        //@include "swserialsingleton.h"
		void update(unsigned long ms) {
            if (swSerial == NULL) {
                swSerial = SoftwareSerialSingleton::get(MHZ19_RXPIN, MHZ19_TXPIN, 9600);
                if (swSerial != NULL) {
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

                AirQualityMsg::GasConcentration level = AirQualityMsg::GasConcentration::MINIMAL;
                if (this->co2 > 2000) {
                    level = AirQualityMsg::GasConcentration::DANGER;
                } else if (this->co2 > 950) {
                    level = AirQualityMsg::GasConcentration::WARNING;
                } else if (this->co2 > 650) {
                    level = AirQualityMsg::GasConcentration::NORM;
                }

                this->sendMessage(new AirQualityMsg(AirQualityMsg::GasType::CO2, 
                    level,
                    (float)this->co2)
                );

                this->pause(CONSUMERPROCESSTIMEOUT);
                return;
            }
		}
    
    protected:
        //@implement
        void sendCommand(uint8_t cmd[], uint8_t* response = NULL) {

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

        //@implement
        void getData() {
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

};

#endif
