#include "ecosense_mhz19sensor.h"
#include "ecosense_cfg.h"

        static uint8_t MHZ19SensorProcess::cmd_getppm[REQUEST_CNT]			= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
        //static uint8_t MHZ19SensorProcess::cmd_zerocalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
        //static uint8_t MHZ19SensorProcess::cmd_spancalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
        //static uint8_t MHZ19SensorProcess::cmd_autocalib_on[REQUEST_CNT] 	= {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
        static uint8_t MHZ19SensorProcess::cmd_autocalib_off[REQUEST_CNT]	= {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};         
MHZ19SensorProcess::MHZ19SensorProcess(int pId, IProcessMessage* msg) : SimpleSensorProcess(pId, msg){
	mhz19active = false;
	//mhz19.begin(MHZ19_RXPIN, MHZ19_TXPIN);
	//mhz19.setAutoCalibration(false);
            MHZ19_writeCommand( MHZ19SensorProcess::cmd_autocalib_off ); 
	MHZ19_getSerialData(STAT);    // первый запрос, в любом случае возвращает -1
            startTime = millis();
            mhz19active = false;
	TRACELNF("MHZ19SensorProcess::init");
}

static IFirmwareProcess* MHZ19SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MHZ19SensorProcess::factory");
	return new MHZ19SensorProcess(pId, msg);
}

void MHZ19SensorProcess::update(unsigned long ms) {
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
