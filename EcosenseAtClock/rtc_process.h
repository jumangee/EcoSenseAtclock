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
        //boolean dotFlag;
		RtcDS3231SimpleI2C	rtc = RtcDS3231SimpleI2C(RTC_I2C_ADDR);

	public:
		PROCESSID(PRC_RTC);
		
		RTClockProcess(IProcessMessage* msg);
        
		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);
};

#endif
