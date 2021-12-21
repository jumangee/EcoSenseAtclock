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
//#define MAIN_FONT Stang5x7
//#define MAIN_FONT fixednums7x15
//#define ICONS_FONT ecosenseicons

#include "LinkedList/LinkedList.h"

#define USE_WARNING_LIGHT 0
#if USE_WARNING_LIGHT == 1
	#define WARNLED_R_PIN A2
	#define WARNLED_G_PIN A1
	#define WARNLED_B_PIN 17

	const byte warnLightGradient[11] PROGMEM = {0, 89, 124, 149, 170, 188, 203, 218, 231, 243, 255};
#endif

struct WarningInfo {
	uint32_t id;
	String title;
	float value;
};

class DisplayProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire oled;
		bool	updateScreen;

		float		temp;
		float		humidity;
		uint16_t	pressure;
		String		time;
		LinkedList<WarningInfo*> warnings;

	public:
		PROCESSID(PRC_DISPLAY);
	
		DisplayProcess(IProcessMessage* msg);

		void addWarning(uint32_t id, String title, float value) {
			int warnPos = this->findWarning(id);
			if (warnPos > -1) {
				this->warnings.get(warnPos)->value = value;
				return;
			}

			WarningInfo* warning = new WarningInfo();
			warning->id = id;
			warning->title = title;
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
		}

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);

		void prn2X(String s) {
			oled.set2X();
			oled.print(s);
			oled.set1X();
		}

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

		#if USE_WARNING_LIGHT == 1
		void updateWarningLight() {
			byte warningLevel = 0;
			warningLevel += this->gasH2S;
			warningLevel += this->gasCH4;
			//warningLevel += this->gasCO2;
			warningLevel += (this->temp > 18 && this->temp < 35) ? 0 : ((this->temp > 40 || this->temp < 12) ? 2 : 1);
			if (humidity > 0)
				warningLevel += (this->humidity > 19 && this->humidity < 50) ? 0 : 1;
			
			/*for (warningLevel = 0; warningLevel < 11; warningLevel++) {
			rgbColor warnLed = warningLevel > 0 ? (rgbColor::ColorMix(rgbColor(0, 255, 0), rgbColor(255, 0, 0),  warningLevel / 10 )) : rgbColor(0, 255, 0);
			TRACEF("ColorMix result: r=")
			TRACE(warnLed.r)
			TRACEF(", g=")
			TRACE(warnLed.g)
			TRACEF(", b=")
			TRACELN(warnLed.b)
			TRACELNF("===")
			}*/

			analogWrite(WARNLED_R_PIN, warnLightGradient[warningLevel]);
			analogWrite(WARNLED_G_PIN, warnLightGradient[11 - warningLevel]);
			analogWrite(WARNLED_B_PIN, 0);
		}
		#endif

		void handleTimeMsg(CurrentTimeMsg* msg);

		void prn(String str) {
			oled.print(str);
		}

		void handleAirQualityMsg(AirQualityMsg* msg);
};

#endif
