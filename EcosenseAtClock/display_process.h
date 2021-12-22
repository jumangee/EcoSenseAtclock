/**
 * Display process
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_MAIN_H
#define _ECOSENSEATCLOCK_MAIN_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "ecosense_messages.h"

#include <Arduino.h>

#define INCLUDE_SCROLLING 0

#include <SSD1306Ascii.h>

#undef OPTIMIZE_I2C
#define OPTIMIZE_I2C 0
#undef INCLUDE_SCROLLING
#define INCLUDE_SCROLLING 0

#include <SSD1306AsciiWire.h>

#include <math.h>

#include "LinkedList/LinkedList.h"

struct WarningInfo {
	uint32_t id;
	float value;
};

class DisplayProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire	oled;
		bool				updateScreen = false;

		float				temp = 0;
		float				humidity = 0;
		uint16_t			pressure = 0;
		uint8_t				timeH = 0;
		uint8_t				timeM = 0;
		bool				timeDots = true;
		LinkedList<WarningInfo*> warnings;
		int					showWarningNum = -1;

	public:
		PROCESSID(PRC_DISPLAY);
	
		DisplayProcess(IProcessMessage* msg);

		void addWarning(uint32_t id, float value) {
			int warnPos = this->findWarning(id);
			if (warnPos > -1) {
				this->warnings.get(warnPos)->value = value;
				return;
			}

			WarningInfo* warning = new WarningInfo();
			warning->id = id;
			warning->value = value;
			warnings.add(warning);

			this->updateScreen = true;
		}

		int findWarning(uint32_t id) {
			for (uint8_t i = 0; i < this->warnings.size(); i++) {
				if (this->warnings.get(i)->id == id) {
					return i;
				}
			}
			return -1;
		}

		void removeWarning(uint32_t id) {
			int warnPos = this->findWarning(id);
			if (warnPos == -1) {
				return;
			}
			delete this->warnings.remove(warnPos);
			this->updateScreen = true;
		}

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);


		void renderMainScreen();

		void renderWarningScreen();

		void render();

		bool handleMessage(IProcessMessage* msg);

		void handleEnvDataMsg(EnvDataMessage* msg);

		void handleTimeMsg(CurrentTimeMsg* msg);

		void handleAirQualityMsg(AirQualityMsg* msg);
};

#endif
