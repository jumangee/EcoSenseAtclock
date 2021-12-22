#include "rtc_process.h"
#include "ecosense_messages.h"

RTClockProcess::RTClockProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
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
	if (!rtc.getIsRunning())
	{
		TRACELNF("WARNING: RTC was not actively running, starting it now.")
		rtc.setIsRunning(true);
	}
	// Reset the DS3231 RTC status in case it was wrongly configured
	rtc.enable32kHzPin(false);
	rtc.disableSquareWavePin();
	TRACELNF("RTClockProcess::READY");
}

static IFirmwareProcess* RTClockProcess::factory(IProcessMessage* msg) {
	return new RTClockProcess(msg);
}

void RTClockProcess::update(unsigned long ms) {
	if (rtc.isDateTimeValid()) {
		datetime_t now = rtc.getNow();
		//dotFlag = !dotFlag;
		this->sendMessage(new CurrentTimeMsg(now.hour, now.min/*, dotFlag*/));
	}
	this->pause(950);
}
