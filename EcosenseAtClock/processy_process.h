/*
* Processy Framework
*/

#ifndef _FIRMWARE_FRAMEWORK_PROCESS_H
#define _FIRMWARE_FRAMEWORK_PROCESS_H

class IProcessMessage;
class IFirmware;

#include "processy_cfg.h"
#include <Arduino.h>

#define PROCESSID(num) const static uint16_t ID = num; uint16_t getId() {return num;}

class IFirmwareProcess {
	public:
		enum ProcessState {
			DUMMY	= 0,
			ACTIVE	= 1,
			PAUSE	= 2,
			STOP	= 3
		};
	private:
 		ProcessState state;

	public:
		IFirmwareProcess(IProcessMessage* msg);

		virtual ~IFirmwareProcess() {
		};

		IFirmwareProcess::ProcessState getState() {
			return this->state;
		}

		static IFirmwareProcess* factory(IProcessMessage* msg);

		/**
		 * @brief Get the Id of the process
		 * 
		 * @return uint16_t 
		 */
		virtual uint16_t getId() {};

		void stop();

		bool isPaused(unsigned long start);

		virtual unsigned long run(unsigned long start);

		virtual void update(unsigned long ms) = 0;

		void pause(unsigned long upTo = 0);

		void unPause();

		virtual bool handleMessage(IProcessMessage* msg);

		void sendMessage(IProcessMessage* msg);

	private:
		//uint16_t processId;
		unsigned long lastUpdate;
		unsigned long pausedUpTo;

		#ifdef DEBUG_PRO_MS
		unsigned long usedMs;
		#endif

	protected:
		IFirmware* getHost();

	public:
		#ifdef DEBUG_PRO_MS
		unsigned long getUsedMs() {
		  return this->usedMs;
		}

		void resetUsedMs() {
		  this->usedMs = 0;
		}
		#endif
};


#endif
