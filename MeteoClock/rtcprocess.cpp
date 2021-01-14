#include "rtcprocess.h"
#include <Arduino.h>

RTClockProcess::RTClockProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
            if (rtc.begin()) {
                this->active = true;
                if (RESET_CLOCK || rtc.lostPower())
                    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));                
                dotFlag = false;
            } else {
                TRACELNF("RTC disabled")
                active = false;
            }
	TRACELNF("RTClockProcess::init");
}

static IFirmwareProcess* RTClockProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("RTClockProcess::factory");
	return new RTClockProcess(pId, msg);
}

		//!@include "processy_cfg.h"
		//!@include "meteo_cfg.h"
void RTClockProcess::update(unsigned long ms) {
	if (!this->active) {
		return;
	}
            dotFlag = !dotFlag;
            /*if (dotFlag) {            // каждую секунду пересчёт времени
                secs++;
                if (secs > 59) {        // каждую минуту
                    secs = 0;
                    mins++;
                }
                if (mins > 59) {        // каждый час*/
                    DateTime now = rtc.now();
                    //secs = now.second();
                    /*if (hrs > 23) hrs = 0;
                }
            }*/
            this->getHost()->sendMessage(new CurrentTimeMsg(now.hour(), now.minute(), dotFlag));
	this->pause(950);
}

RTClockProcess::~RTClockProcess() {
	// stop process
	TRACELNF("RTClockProcess::stop");
}
