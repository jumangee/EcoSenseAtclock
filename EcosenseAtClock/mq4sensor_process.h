/**
 * MQ-4 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ4_SENSOR_PROCESS_H
#define _MQ4_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"
#include "mqsensor_process.h"

//#define MQ136_RL 2.3
//#define MQ136_FRESH_RO 1

class MQ4SensorProcess: public MQSensorProcess {
	public:
		PROCESSID(PRC_MQ4);
		
		MQ4SensorProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		/*//@implement
		//!@include "ecosense_cfg.h"
		//!@include "mqsensor_process.h"
		void update(unsigned long ms) {
			if (!readingsDone(MQ4_ANALOG_PIN, READINGS_PER_RESULT)) {
				return;
			}

			{
				TRACEF("[ MQ-4 ] analog=");
				TRACE(this->getValue());
				//TRACEF(", dig=");
				//TRACE(this->mq136dig);
				TRACEF(", V=");
				TRACE( this->getVoltage() );
				TRACEF(", instant=");
				TRACE( this->instantValue(MQ4_ANALOG_PIN) );
				TRACEF(", Dig=");
				TRACELN( digitalRead(MQ4_DIGITAL_PIN) );
			}

			this->getHost()->sendMessage(new AirQualityMsg(CH4, this->getQuality(.4), this->getVoltage()));

			this->pause(ENVSENSORS_TIMEOUT);
		}*/

		IProcessMessage* getResultMsg();

};

#endif