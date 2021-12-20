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

#define WAIT_READ_TIMES	100
#define WAIT_READ_DELAY	10

#define MHZ19_RXPIN 4
#define MHZ19_TXPIN 5
#define MHZ19_CMDSIZE 9

#include "SoftwareSerial.h"

class MHZ19SensorProcess: public IFirmwareProcess {
    protected:
		bool			mhz19active;
        SoftwareSerial  swSerial = SoftwareSerial(MHZ19_RXPIN, MHZ19_TXPIN);

        int co2 = 0;
        int temp = 0;
        int status = -1;

        static uint8_t CMD_GETPPM[MHZ19_CMDSIZE];
        static uint8_t CMD_SETRNG[MHZ19_CMDSIZE];
        static uint8_t CMD_AUTOCALOFF[MHZ19_CMDSIZE];

        //@cpp
        static uint8_t MHZ19SensorProcess::CMD_GETPPM[MHZ19_CMDSIZE] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
        //@cpp
        static uint8_t MHZ19SensorProcess::CMD_SETRNG[MHZ19_CMDSIZE] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};
        //@cpp
        static uint8_t MHZ19SensorProcess::CMD_AUTOCALOFF[MHZ19_CMDSIZE] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};         

	public:
        PROCESSID(PRC_MHZ19);

		//@implement
        //@include "SoftwareSerial.h"
		MHZ19SensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			mhz19active = true;
			
            swSerial.begin(9600);

            sendCommand( MHZ19SensorProcess::CMD_AUTOCALOFF );
            sendCommand( MHZ19SensorProcess::CMD_SETRNG );
			getData();    // первый запрос, в любом случае возвращает -1

            // pre-burn timeout
            this->pause(60000);    //mhz19
            TRACELNF("MHZ19SensorProcess pre-burn timeout")
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new MHZ19SensorProcess(msg);
		}

		//@implement
        //@include "ecosense_messages.h"
		void update(unsigned long ms) {
            getData();

            if (this->status != -1) {
                /*TRACEF("co2=")
                TRACE(this->co2)
                TRACEF(", co2*2/5=")
                TRACE(this->co2*2/5)
                TRACEF(", temp=")
                TRACELN(this->temp)*/

                this->sendMessage(new AirQualityMsg(AirQualityMsg::GasType::CO2, 
                    this->co2 > 2500 ? AirQualityMsg::GasConcentration::DANGER : (
                        this->co2 > 1000 ? AirQualityMsg::GasConcentration::WARNING : (
                            this->co2 > 600 ? AirQualityMsg::GasConcentration::NORM : AirQualityMsg::GasConcentration::MINIMAL
                        )
                    ),
                    this->co2));
                this->sendMessage(new TaskDoneMessage(this));
            }
            
			this->pause(12000);
		}
    
    protected:
        //@implement
        void sendCommand(uint8_t cmd[], uint8_t* response = NULL) {

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

            this->co2 = (256*((unsigned int) buf[2])) + ((unsigned int) buf[3]);
            this->temp = (buf[4]-32)*5/9;
            this->status = buf[5];
        }	 

};

#endif
