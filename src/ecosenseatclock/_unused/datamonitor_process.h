/**
 * Data monitoring process
 * for Processy Firmware Framework
 */
 
#ifndef _MONITOR_PROCESS_H
#define _MONITOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_messages.h"

class DataMonitorProcess: public IFirmwareProcess {
	public:
		PROCESSID(PRC_DATAREPORT);
		
		//@implement
		//@include "ecosense_cfg.h"
		DataMonitorProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
		}

		//@implement
		void update(unsigned long ms) {
			this->pause(2500);	// no need to be updated
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new DataMonitorProcess(msg);
		}

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					this->handleEnvDataMsg((EnvDataMessage*)msg);
					break;
				}
				case AIRQUALITY_MESSAGE: {
					this->handleAirQualityMsg((AirQualityMsg*)msg);
					break;
				}
			}
			return false;
		}

		/**
		 * Convert data to thingspeak field value
		 * Environment
		 */
		void handleEnvDataMsg(EnvDataMessage* msg) {
			if (msg->isActive()) {
				ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL1, 1, msg->getTempF());

				if (msg->getHumidity() > 0) {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL1, 4, msg->getHumidity());
				}
				if (msg->getPressure() > 0) {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL1, 5, msg->getPressure());
				}
			}
		}

		/**
		 * Convert data to thingspeak field value
		 * Air quality
		 */
		void handleAirQualityMsg(AirQualityMsg* msg) {
			switch (msg->gasType())
			{
				case AirQualityGasType::COMMON : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 1, msg->getVoltage());	// MQ-135
					break;
				}
				case AirQualityGasType::CO2: {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 2, msg->getVoltage());	// MH-Z19
					break;
				}
				case AirQualityGasType::CO : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 3, msg->getVoltage());	// MQ-7
					break;
				}
				case AirQualityGasType::PM1 : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 4, msg->getVoltage());	// PPD42
					break;
				}
				case AirQualityGasType::PM25 : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 5, msg->getVoltage());	// PPD42
					break;
				}
				case AirQualityGasType::H2S : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 6, msg->getVoltage());	// MQ-136
					break;
				}
				case AirQualityGasType::CH2O : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 7, msg->getVoltage());	// ZE08-CH2O
					break;
				}
				case AirQualityGasType::CH4 : {
					ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 8, msg->getVoltage());	// MQ-4
					break;
				}
				case AirQualityGasType::VOCs : {
					//ThingspeakFieldMessage(ThingspeakFieldMessage::ThingspeakChannel::CHANNEL2, 8, msg->getVoltage());	// CJMCU-1100
					break;
				}
			}
		}
};

#endif
