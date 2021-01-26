/**
 * Data monitoring process
 * for Processy Firmware Framework
 */
 
#ifndef _MONITOR_PROCESS_H
#define _MONITOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"

#include "LinkedList/LinkedList.h"

class DataMonitorProcess: public IFirmwareProcess {
	private:
		LinkedList<IProcessMessage*> msgBuffer;

	public:
		//@implement
		//@include "ecosense_cfg.h"
		DataMonitorProcess(int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
		}

		//@implement
		void update(unsigned long ms) {
			this->pause(ENVSENSORS_TIMEOUT);	// no need to be updated
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("DataMonitorProcess::factory");
			return new DataMonitorProcess(pId, msg);
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
			//
		}

		/**
		 * Convert data to thingspeak field value
		 * Air quality
		 */
		void handleAirQualityMsg(AirQualityMsg* msg) {
			//
		}
};

#endif
