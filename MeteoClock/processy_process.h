/*
* Processy Framework
*/

#ifndef _FIRMWARE_FRAMEWORK_PROCESS_H
#define _FIRMWARE_FRAMEWORK_PROCESS_H

class IProcessMessage;
class IFirmware;

#include "processy_cfg.h"
//#include <WString.h>

class IFirmwareProcess {
	public:
		IFirmwareProcess(int pId, IProcessMessage* msg);

		~IFirmwareProcess();

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		int getId() {
			return this->processId;
		}

		bool isId(int compareTo);

		unsigned long run(unsigned long start);

		virtual void update(unsigned long ms) = 0;

		void pause(unsigned long upTo = 0);

		void unPause();

		virtual bool handleMessage(IProcessMessage* msg);

	private:
		int processId;
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
