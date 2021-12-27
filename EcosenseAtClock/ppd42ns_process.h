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


class ParticlePPD42SensorProcess: public ADCMuxChannelProcess {
	private:
		float ratioPm1;
		float ratioPm2;
		float concentrationPm1;
		float concentrationPm2;

		bool statePm1 = true;

		unsigned long lowpulseoccupancy = 0;
		unsigned long sampleTotalTime = 0;

	public:
		PROCESSID(PRC_PPD42NS);

		ParticlePPD42SensorProcess();

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);
};

#endif
