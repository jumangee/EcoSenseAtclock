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
		//@implement
		RTClockProcess(uint16_t pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
            dotFlag = false;
            if (rtc.begin()) {
                this->active = true;

                if (RESET_CLOCK || rtc.lostPower())
                    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
				// TODO: http://worldtimeapi.org/api/timezone/Europe/Moscow.txt

				TRACELNF("[RTC] OK")
            } else {
                TRACELNF("[RTC] FAIL")
                active = false;
            }

			TRACELNF("RTClockProcess::init");
		}
        
		//@implement
		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg) {
			TRACELNF("RTClockProcess::factory");
			return new RTClockProcess(pId, msg);
		}

        //@implement
		//!@include "processy_cfg.h"
		//!@include "meteo_cfg.h"
		void update(unsigned long ms) {
			if (!this->active) {
				return;
			}

            dotFlag = !dotFlag;
            DateTime now = rtc.now();
			//TRACEF("[RTC] Time: ")
			//TRACELN(now.timestamp())
            this->getHost()->sendMessage(new CurrentTimeMsg(now.hour(), now.minute(), dotFlag));
			#ifdef RTC_GET_TEMPERATURE
            if (!dotFlag) {
				//TRACEF("[RTC] TEMPERATURE: ")
				//TRACELN(rtc.getTemperature())
                this->getHost()->sendMessage(new EnvDataMessage(rtc.getTemperature() - 1.4, 0, 0));
			}
			#endif

			this->pause(950);
		}

		//@implement
		//@include <Arduino.h>
		~RTClockProcess() {
			// stop process
			TRACELNF("RTClockProcess::stop");
		}

};

#endif
