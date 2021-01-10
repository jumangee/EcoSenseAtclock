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
		//@implement
		//@include "processy_cfg.h"
		//@include "stuff.h"
		//@include <Arduino.h>
		IFirmwareProcess(String id, IProcessMessage* msg) {
			this->processId = id;
			this->lastUpdate = millis();
			#ifdef DEBUG_PRO_MS
			this->resetUsedMs();
			#endif
			//TRACE(S("IFirmwareProcess::",this->processId.c_str(),"/", (this->pausedUpTo == NULL ? "NULL" : (String(*this->pausedUpTo)).c_str()) ))
			this->unPause();
		}

		//@implement
		~IFirmwareProcess() {
			this->unPause();
		}

		String getId() {
			return processId;
		}

		//@implement
    	//@include "processy.h"
		void log(String msg){
			this->getHost()->log(msg);
		}

		//@implement
		bool isId(String compareTo) {
			return this->processId == compareTo;
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "stuff.h"
		//@include <Arduino.h>
		unsigned long run(unsigned long start) {
			unsigned long ms = start - this->lastUpdate;
			//TRACE(S("IFirmwareProcess::run/",this->processId.c_str(),"/", (this->pausedUpTo == NULL ? "NULL" : (String(*this->pausedUpTo)).c_str()) ))
			if (this->pausedUpTo != NULL && start < this->pausedUpTo) {
				return start;	// no time wasting
			} else if (this->pausedUpTo) {
				this->unPause();
			}
			this->update(ms);
			unsigned long endTime = millis();
			#ifdef DEBUG_PRO_MS
			this->usedMs += endTime - start;
			#endif
			this->lastUpdate = endTime;
			return endTime;
		}

		virtual void update(unsigned long ms) = 0;

		//@implement
		void pause(unsigned long upTo = 0) {
			this->pausedUpTo = new unsigned long(millis() + upTo);
		}

		//@implement
		void unPause() {
			if (this->pausedUpTo != NULL) {
				delete this->pausedUpTo;
				this->pausedUpTo = NULL;
			}
		}

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			return false;	// override this
		}

	private:
		String processId;
		unsigned long lastUpdate;
		unsigned long *pausedUpTo;

		#ifdef DEBUG_PRO_MS
		unsigned long usedMs;
		#endif

	protected:
		//@implement
		IFirmware* getHost(){
			return IFirmware::get();
		}

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
