/*
* Processy Framework
*/

#ifndef _FIRMWARE_FRAMEWORK_PROCESS_H
#define _FIRMWARE_FRAMEWORK_PROCESS_H

class IProcessMessage;
class IFirmware;

#include "processy_cfg.h"
#include <WString.h>

class IFirmwareProcess {
	public:
		IFirmwareProcess(String id, IProcessMessage* msg);

		~IFirmwareProcess();

		String getId() {
			return processId;
		}

		void log(String msg);

		bool isId(String compareTo);

		unsigned long run(unsigned long start);

		virtual void update(unsigned long ms) = 0;

		void pause(unsigned long upTo = 0);

		void unPause();

		bool handleMessage(IProcessMessage* msg);

	private:
		String processId;
		unsigned long lastUpdate;
		unsigned long *pausedUpTo;

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
