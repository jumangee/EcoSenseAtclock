/**
 * Real Time Clock process
 * for Processy Firmware Framework
 * 
 */

#ifndef _RTC_PROCESS_H
#define _RTC_PROCESS_H

#include "processy.h"
#include "processy_process.h"
#include "meteo_messages.h"

#include "RTClib.h"

#define RESET_CLOCK 0

class RTClockProcess: public IFirmwareProcess {
	private:
        RTC_DS3231 rtc;
        //DateTime now;
        //int8_t hrs, mins, secs;
        boolean dotFlag;

        bool active;

	public:
		RTClockProcess(int pId, IProcessMessage* msg);
        
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		void update(unsigned long ms);

		~RTClockProcess();

};

#endif
