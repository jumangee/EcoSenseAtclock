/*
* Firmware Process Control Framework (FFPro)
*
*/
 
#ifndef _FIRMWARE_FRAMEWORK_PROCESS_H
#define _FIRMWARE_FRAMEWORK_PROCESS_H

class IProcessMessage;
class IFirmware;

//extern unsigned long millis();

#include "ffpro_cfg.h"
#include <WString.h>
#include <Arduino.h>

class IFirmwareProcess {
	public:
		IFirmwareProcess(String id, IProcessMessage* msg) {
			this->processId = id;
			this->lastUpdate = millis();
			#ifdef DEBUG_PRO_MS
			this->resetUsedMs();
			#endif
		}
		
		String getId() {
			return processId;
		}
		
		void log(String msg);  // in cpp
		
		bool isId(String compareTo) {
			return this->processId == compareTo; 
		}
		
		unsigned long run(unsigned long start) {
			unsigned long ms = start - this->lastUpdate;
			if (this->pausedUpTo != NULL && start < this->pausedUpTo) {
				return start;	// no time wasting
			} else if (this->pausedUpTo != NULL && this->pausedUpTo > 0) {
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
		
		void pause(unsigned long upTo = 0) {
			this->pausedUpTo = new unsigned long(millis() + upTo);
			//this->pausedUpTo = ;
		}
		
		void unPause() {
			delete this->pausedUpTo;
			this->pausedUpTo = NULL;
		}
		
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
		IFirmware* getHost();	// in cpp

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
