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

#include "MHZ19_uart/MHZ19_uart.h"

class MHZ19SensorProcess: public SimpleSensorProcess {
		MHZ19_uart		mhz19;
		bool			mhz19active;
        uint32_t        startTime;

	public:
		//@implement
		//@include "ecosense_cfg.h"
		MHZ19SensorProcess(int pId, IProcessMessage* msg): SimpleSensorProcess(pId, msg) {
			mhz19active = false;
			mhz19.begin(MHZ19_RXPIN, MHZ19_TXPIN);
			mhz19.setAutoCalibration(false);
			mhz19.getStatus();    // первый запрос, в любом случае возвращает -1
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
            return mhz19.getPPM();
        }

		//@implement
		//@include "ecosense_cfg.h"
		void update(unsigned long ms) {
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

};

#endif
