/**
 * MHZ19 sensor process
 * for Processy Firmware Framework
 */
 
#ifndef _MHZ19_SENSOR_PROCESS_H
#define _MHZ19_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "ecosense_cfg.h"
#include "simple_sensor_process.h"

#include "SoftwareSerial.h"

#define REQUEST_CNT 8
#define RESPONSE_CNT 9
#define WAIT_READ_TIMES	100
#define WAIT_READ_DELAY	10

//#include "MHZ19_uart/MHZ19_uart.h"

class MHZ19SensorProcess: public SimpleSensorProcess {
		//MHZ19_uart		mhz19;
		bool			mhz19active;
        uint32_t        startTime;

        enum MHZ19_DATA { 
            PPM, TEMPERATURE, STAT 
        };

        static uint8_t cmd_getppm[REQUEST_CNT];
        /*static uint8_t cmd_zerocalib[REQUEST_CNT];
        static uint8_t cmd_spancalib[REQUEST_CNT];
        static uint8_t cmd_autocalib_on[REQUEST_CNT];*/
        static uint8_t cmd_autocalib_off[REQUEST_CNT];

        //@cpp
        static uint8_t MHZ19SensorProcess::cmd_getppm[REQUEST_CNT]			= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
        //@cpp
        //static uint8_t MHZ19SensorProcess::cmd_zerocalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
        //@cpp
        //static uint8_t MHZ19SensorProcess::cmd_spancalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
        //@cpp
        //static uint8_t MHZ19SensorProcess::cmd_autocalib_on[REQUEST_CNT] 	= {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
        //@cpp
        static uint8_t MHZ19SensorProcess::cmd_autocalib_off[REQUEST_CNT]	= {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};         

	public:
		//@implement
		//@include "ecosense_cfg.h"
		MHZ19SensorProcess(int pId, IProcessMessage* msg): SimpleSensorProcess(pId, msg) {
			mhz19active = false;
			//mhz19.begin(MHZ19_RXPIN, MHZ19_TXPIN);
			//mhz19.setAutoCalibration(false);
            MHZ19_writeCommand( MHZ19SensorProcess::cmd_autocalib_off ); 
			MHZ19_getSerialData(STAT);    // первый запрос, в любом случае возвращает -1
            startTime = millis();
            mhz19active = false;

			TRACELNF("MHZ19SensorProcess::init");
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("MHZ19SensorProcess::factory");
			return new MHZ19SensorProcess(pId, msg);
		}

        uint16_t getInstantValue() {
            return MHZ19_getSerialData(PPM);
        }

		//@implement
		//@include "ecosense_cfg.h"
		void update(unsigned long ms) {
            if (!mhz19active) {
                if (millis() - this->startTime < 1000) {
                    return;
                }

                if (MHZ19_getSerialData(STAT) == 0) {
                    mhz19active = true;
                    return;
                } else {
                    // error
                    this->getHost()->sendMessage(new ProcessStateMsg(this->getId(), IFirmwareProcess::ProcessState::STOP));
                    this->stop();
                }
            }

            if (!this->readingsDone(READINGS_PER_RESULT)) {
                return;
            }

            uint16_t ppm = getValue();
            byte q = 0;
            if (ppm < 600) {
                q = 42 * ppm / 600;
            } else if (ppm < 1000) {
                q = 84 * ppm / 1000;
            } else {
                q = 127 * ppm / 10000;
            }
            this->getHost()->sendMessage(new AirQualityMsg(CO2, q, this->getVoltage()));
            
			this->pause(ENVSENSORS_TIMEOUT);
		}

        void MHZ19_writeCommand(uint8_t cmd[]) {
            MHZ19_writeCommand(cmd, NULL);
        }

        void MHZ19_writeCommand(uint8_t cmd[], uint8_t* response) {
            SoftwareSerial sserial(MHZ19_RXPIN, MHZ19_TXPIN);
            sserial.begin(9600);

            sserial.write(cmd, REQUEST_CNT);
            sserial.write(MHZ19_checksum(cmd));
            sserial.flush();
            
            if (response != NULL) {
                int i = 0;
                while(sserial.available() <= 0) {
                    if( ++i > WAIT_READ_TIMES ) {
                        TRACELNF("error: can't get MH-Z19 response.");
                        return;
                    }
                    yield();
                    delay(WAIT_READ_DELAY);
                }
                sserial.readBytes(response, RESPONSE_CNT);
            }
        }

        uint8_t MHZ19_checksum( uint8_t com[] ) {
            uint8_t sum = 0x00;
            for ( int i = 1; i < REQUEST_CNT; i++) {
                sum += com[i];
            }
            sum = 0xff - sum + 0x01;
            return sum;
        }

        int MHZ19_getSerialData(MHZ19_DATA flg) {
            uint8_t buf[RESPONSE_CNT];
            for( int i=0; i<RESPONSE_CNT; i++){
                buf[i]=0x0;
            }

            MHZ19_writeCommand(MHZ19SensorProcess::cmd_getppm, buf);
            int co2 = 0, co2temp = 0, co2status =  0;

            // parse
            if (buf[0] == 0xff && buf[1] == 0x86 && MHZ19_checksum(buf) == buf[RESPONSE_CNT-1]) {
                co2 = buf[2] * 256 + buf[3];
                co2temp = buf[4] - 40;
                co2status =  buf[5];
            } else {
                co2 = co2temp = co2status = -1;
            }

            switch(flg) {
                case MHZ19_DATA::TEMPERATURE:
                    return co2temp;
                    break;
                case MHZ19_DATA::STAT:
                    return co2status;
                    break;
                case MHZ19_DATA::PPM:
                default:
                    return co2;
                    break;
            }
        }	 

};

#endif
