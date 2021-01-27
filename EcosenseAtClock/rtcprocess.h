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
//#include "RTClib.h"

#include "RTC/rtc_ds3231simple.h"
#include <Arduino.h>
//#include <math.h>

class RTClockProcess: public IFirmwareProcess {
	private:
        //RTC_DS3231 rtc;
        //DateTime now;
        //int8_t hrs, mins, secs;
        boolean dotFlag;

        bool active;

		RtcDS3231SimpleI2C	rtc = RtcDS3231SimpleI2C(RTC_I2C_ADDR);

	public:
		RTClockProcess(uint16_t pId, IProcessMessage* msg);

		~RTClockProcess();
        
		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

		void update(unsigned long ms);
};

#endif
