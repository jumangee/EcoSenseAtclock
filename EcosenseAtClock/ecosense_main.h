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
#include "ecosenseicons.h"
#include "stuff.h"

#define OLED_ADDR   0x3C

//#define MAIN_FONT System5x7
//#define MAIN_FONT Stang5x7
#define MAIN_FONT fixednums7x15
#define ICONS_FONT ecosenseicons

#define SPRITE_GAS_CH4	0x2A
#define SPRITE_GAS_H2S	0x24
#define SPRITE_GAS_CO2	0x2B
#define SPRITE_OK		0x2C
#define SPRITE_WARNING	0x21
#define SPRITE_DANGER	0x2E
#define SPRITE_SPC		0x20
#define SPRITE_WIFI		0x22
#define SPRITE_ENV_TEMP	0x28
#define SPRITE_ENV_C	0x29
#define SPRITE_ENV_HUM	0x23
#define SPRITE_ENV_PRES	0x2D

#define SCREENROW_GAS_H2S 2
#define SCREENROW_GAS_CH4 4
#define SCREENROW_GAS_CO2 6

#define USE_WARNING_LIGHT 1
#if USE_WARNING_LIGHT == 1
	#define WARNLED_R_PIN A2
	#define WARNLED_G_PIN A1
	#define WARNLED_B_PIN 17

	const byte warnLightGradient[11] PROGMEM = {0, 89, 124, 149, 170, 188, 203, 218, 231, 243, 255};
#endif

//#define PHOTORESISTOR_PIN A3

class MainProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire oled;
		bool	updateScreen;

		int		temp;
		byte	humidity;
		int		pressure;
		byte	gasH2S;
		byte	gasCH4;

		bool	clocktick;

	public:
		MainProcess(uint16_t pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

		~MainProcess();

		void update(unsigned long ms);

		void prn2X(String s) {
			oled.set2X();
			oled.print(s);
			oled.set1X();
		}

		bool prnEnvData(char spr, byte row, byte value) {
			if (value > 0) {
				oled.setFont(ICONS_FONT);
				oled.setCursor(0, row);
				prn(spr);
				prn(SPRITE_SPC);
				oled.setFont(MAIN_FONT);
				oled.print(value);
				///prn2X(S(value));
				//oled.set2X();
				//oled.print(value);
				/*} else {
					oled.print(F("-   "));
				}*/
				//oled.set1X();
				return true;
			}
			return false;
		}

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

		void prn(char c) {
			oled.print(c);
		}

		void printGasInfo(char g, byte row, byte quality);

		void handleAirQualityMsg(AirQualityMsg* msg);

		void handleWifiMsg(WiFiStateMsg* msg);
};

#endif
