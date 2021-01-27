#include "ecosense_mhz19sensor.h"
#include "ecosense_cfg.h"

MHZ19SensorProcess::MHZ19SensorProcess(int pId, IProcessMessage* msg) : SimpleSensorProcess(pId, msg){
	mhz19active = false;
	mhz19.begin(MHZ19_RXPIN, MHZ19_TXPIN);
	mhz19.setAutoCalibration(false);
	mhz19.getStatus();    // первый запрос, в любом случае возвращает -1
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
                if (mhz19.getStatus() == 0) {
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
