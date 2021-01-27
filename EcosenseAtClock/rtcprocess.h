/**
 * Real Time Clock process
 * for Processy Firmware Framework
 * 
 */

#ifndef _RTC_PROCESS_H
#define _RTC_PROCESS_H

#include "processy.h"
#include "processy_process.h"
#include "ecosense_messages.h"

#include "ecosense_cfg.h"
#include "RTClib.h"

class RTClockProcess: public IFirmwareProcess {
	private:
        RTC_DS3231 rtc;
        //DateTime now;
        //int8_t hrs, mins, secs;
        boolean dotFlag;

        bool active;

	public:
		RTClockProcess(uint16_t pId, IProcessMessage* msg);
        
		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

		void update(unsigned long ms);

		~RTClockProcess();

};

#endif
