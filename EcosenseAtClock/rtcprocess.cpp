#include "rtcprocess.h"

RTClockProcess::RTClockProcess(uint16_t pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
            dotFlag = false;
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

RTClockProcess::~RTClockProcess() {
	//delete this->Rtc;
}

static IFirmwareProcess* RTClockProcess::factory(uint16_t pId, IProcessMessage* msg) {
	TRACELNF("RTClockProcess::factory");
	return new RTClockProcess(pId, msg);
}

		//!@include "processy_cfg.h"
		//!@include "meteo_cfg.h"
void RTClockProcess::update(unsigned long ms) {
	if (!this->active) {
		return;
	}
	if (rtc.isDateTimeValid()) {
		datetime_t now = rtc.getNow();
		this->getHost()->sendMessage(new CurrentTimeMsg(now.hour, now.min, dotFlag));
	}
            /*dotFlag = !dotFlag;
            DateTime now = Rtc->now();
	//TRACEF("[RTC] Time: ")
	//TRACELN(now.timestamp())
            this->getHost()->sendMessage(new CurrentTimeMsg(now.hour(), now.minute(), dotFlag));
	#ifdef RTC_GET_TEMPERATURE
            if (!dotFlag) {
		//TRACEF("[RTC] TEMPERATURE: ")
		//TRACELN(Rtc->getTemperature())
                this->getHost()->sendMessage(new EnvDataMessage(Rtc->getTemperature() - 1.4, 0, 0));
	}
	#endif*/
	this->pause(950);
}
