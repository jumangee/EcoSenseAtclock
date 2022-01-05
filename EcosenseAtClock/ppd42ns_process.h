/**
 * PPD42NS air quality sensor process
 * for Processy Firmware Framework
 */

#ifndef _PPD42NS_PROCESS_H
#define _PPD42NS_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include <Arduino.h>

#include "adcmuxchannel_process.h"


class PPD42SensorProcess: public ADCMuxChannelProcess {
	private:
		unsigned long	lowpulseoccupancy = 0;
		unsigned long	sampleTotalTime = 0;
		float			concentration = 0;
		float			ratio = 0;

	public:
		PROCESSID(PRC_PPD42NS);

		PPD42SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);

		bool getConcentration(byte pin) {
			if (this->getChannel(pin)) {
				unsigned long starttime = millis();
				this->lowpulseoccupancy += pulseIn(pin, LOW, 1000);
				this->freeChannel();

				this->sampleTotalTime += millis()-starttime;

				if ((this->sampleTotalTime) > PPD42NS_SAMPLE_TIME_MS)	{
					ratio = this->lowpulseoccupancy/(PPD42NS_SAMPLE_TIME_MS*10.0);  // Integer percentage 0=>100
					concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

					this->lowpulseoccupancy = 0;
					this->sampleTotalTime = 0;

					return true;
				}
			}

			return false;
		}
};

#endif
