/**
 * Real Time Clock process
 * for Processy Firmware Framework
 * 
 */

#ifndef _RTC_PROCESS_H
#define _RTC_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "ecosense_cfg.h"

#include "RTC/rtc_ds3231simple.h"
#include <Arduino.h>
//#include <math.h>

class RTClockProcess: public IFirmwareProcess {
	private:
        //boolean dotFlag;
		RtcDS3231SimpleI2C	rtc = RtcDS3231SimpleI2C(RTC_I2C_ADDR);

	public:
		PROCESSID(PRC_RTC);
		
		//@implement
		RTClockProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
            //dotFlag = false;

			#if RESET_CLOCK == 1
				const char* compile_date_time = __DATE__ " " __TIME__;
				datetime_t compiled_time_t = rtc.str20ToTime(compile_date_time);
				// Check if the time is valid.
				//if (!Rtc->isDateTimeValid())
				//{
					rtc.setTime(&compiled_time_t);
				//}
			#endif

			// Check if the RTC clock is running (Yes, it can be stopped, if you wish!)
			/*if (!rtc.getIsRunning())
			{
				TRACELNF("WARNING: RTC was not actively running, starting it now.")
				rtc.setIsRunning(true);
			}*/

			// Reset the DS3231 RTC status in case it was wrongly configured
			rtc.enable32kHzPin(false);
			rtc.disableSquareWavePin();

			TRACELNF("RTClockProcess::READY");
		}
        
		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new RTClockProcess(msg);
		}

        //@implement
		//@include "ecosense_messages.h"
		void update(unsigned long ms) {
			if (rtc.isDateTimeValid()) {
				datetime_t now = rtc.getNow();
				//dotFlag = !dotFlag;
				this->sendMessage(new CurrentTimeMsg(now.hour, now.min/*, dotFlag*/));
			}

			this->pause(950);
		}
};

#endif
