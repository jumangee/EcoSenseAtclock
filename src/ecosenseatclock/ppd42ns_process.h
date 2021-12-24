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

		//@implement
		ParticlePPD42SensorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			TRACELNF("ParticlePPD42SensorProcess: pre-burn timeout")
			this->pause(60000);
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new ParticlePPD42SensorProcess(msg);
		}

		//@implement
		//@include "ecosense_messages.h"
		void done() {
			// calc average result

			float resultRatioPm1 = 0;
			float resultRatioPm2 = 0;
			float resultConcPm1 = 0;
			float resultConcPm2 = 0;
			for (byte i = 0; i < PPD42NS_READS_COUNT; i++) {
				resultRatioPm1 += samples[i].ratioPm1;
				resultRatioPm2 += samples[i].ratioPm2;
				resultConcPm1 += samples[i].concentrationPm1;
				resultConcPm2 += samples[i].concentrationPm2;
			}

			this->sendMessage(new ParticlePPD42Message(resultRatioPm1 / PPD42NS_READS_COUNT, resultConcPm1 / PPD42NS_READS_COUNT, resultRatioPm2 / PPD42NS_READS_COUNT, resultConcPm2 / PPD42NS_READS_COUNT));
			this->sendMessage(new TaskDoneMessage(this));

			this->pause();
		}

        //@implement
		//@include "swserialsingleton.h"
		//@include "adcmux_mngmnt.h"
		void update(unsigned long ms) {
			if (!SoftwareSerialSingleton::isfree()) {	// better not to test in parallel with intercept-blocking operations
				pause(250);
				return;
			}

			if (this->currentMeasureMode == 0) {
				this->currentMeasureMode = MUXCHANNEL_PM1;
			}

			uint32_t ticket = ADCMuxManagement::get()->requestChannel(this->currentMeasureMode);	// ADC MUX PIN!
			if (ticket) {
				unsigned long starttime = millis();
				this->lowpulseoccupancy += pulseIn(this->currentMeasureMode, LOW, 1000);
				ADCMuxManagement::get()->release(ticket);

				this->sampleTotalTime += millis()-starttime;

				if ((this->sampleTotalTime) > PPD42NS_SAMPLE_TIME_MS)
				{
					float ratio = this->lowpulseoccupancy/(PPD42NS_SAMPLE_TIME_MS*10.0);  // Integer percentage 0=>100
					float concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
					
					/*TRACEF("mode: ");
					TRACE(this->currentMeasureMode);

					TRACEF(", lowpulseoccupancy: ");
					TRACE(lowpulseoccupancy);
					
					TRACEF(", ratio: ");
					TRACE(ratio);

					TRACEF(", concentration: ");
					TRACELN(concentration);*/

					if (this->currentMeasureMode == MUXCHANNEL_PM1) {
						samples[this->currentSample].ratioPm1 = ratio;
						samples[this->currentSample].concentrationPm1 = concentration;
						this->currentMeasureMode = MUXCHANNEL_PM2;
					} else {
						samples[this->currentSample].ratioPm2 = ratio;
						samples[this->currentSample].concentrationPm2 = concentration;
						this->currentMeasureMode = MUXCHANNEL_PM1;

						this->currentSample++;
						if (this->currentSample >= PPD42NS_READS_COUNT) {
							this->currentSample = 0;

							//stop sampling and process
							this->done();
						}
					}

					this->lowpulseoccupancy = 0;
					this->sampleTotalTime = 0;
				}
			}
		}
};

#endif
