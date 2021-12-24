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


class ParticlePPD42SensorProcess: public IFirmwareProcess {
	private:
		byte currentSample = 0;
		byte currentMeasureMode = 0;

		struct PPD42SampleData {
			float ratioPm1;
			float ratioPm2;
			float concentrationPm1;
			float concentrationPm2;
		};

		PPD42SampleData samples[PPD42NS_READS_COUNT];

		unsigned long lowpulseoccupancy = 0;
		unsigned long sampleTotalTime = 0;

	public:
		PROCESSID(PRC_PPD42NS);

		ParticlePPD42SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void done();

		void update(unsigned long ms);
};

#endif
