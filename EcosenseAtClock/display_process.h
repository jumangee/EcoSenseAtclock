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
//#include "ecosenseicons.h"
#include "stuff.h"

#define OLED_ADDR   0x3C

#define MAIN_FONT System5x7
//#define MAIN_FONT font5x7
//#define MAIN_FONT fixednums7x15
//#define ICONS_FONT ecosenseicons

#include "LinkedList/LinkedList.h"

struct WarningInfo {
	uint32_t id;
	float value;
};

class DisplayProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire oled;
		bool	updateScreen;

		float		temp;
		float		humidity;
		uint16_t	pressure;
		uint8_t		timeH;
		uint8_t		timeM;
		bool		timeDots;
		LinkedList<WarningInfo*> warnings;

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

		/*void prn2X(String s) {
			oled.set2X();
			oled.print(s);
			oled.set1X();
		}*/

		/*bool prnEnvData(char spr, byte row, byte value) {
			if (value > 0) {
				oled.setFont(ICONS_FONT);
				oled.setCursor(0, row);
				prn(spr);
				prn(SPRITE_SPC);
				oled.setFont(MAIN_FONT);
				oled.print(value);
				return true;
			}
			return false;
		}*/

		void render();

		bool handleMessage(IProcessMessage* msg);

		void handleEnvDataMsg(EnvDataMessage* msg);

		void handleTimeMsg(CurrentTimeMsg* msg);

		void handleAirQualityMsg(AirQualityMsg* msg);
};

#endif
