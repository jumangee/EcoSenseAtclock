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

        /*//@cpp
        static uint8_t MHZ19SensorProcess::CMD_RESET[MHZ19_CMDSIZE] = {0xff, 0x01, 0x8d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71};*/

	public:
        PROCESSID(PRC_MHZ19);

		MHZ19SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);
    
    protected:
        void sendCommand(uint8_t cmd[], uint8_t* response = NULL);

        void getData();

};

#endif
