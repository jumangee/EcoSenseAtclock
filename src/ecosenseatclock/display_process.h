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
	
		//@implement
		//@include <SSD1306AsciiWire.h>
		DisplayProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			TRACELNF("DisplayProcess::start");

			#if USE_WARNING_LIGHT == 1
			pinMode(WARNLED_R_PIN, OUTPUT);
			pinMode(WARNLED_G_PIN, OUTPUT);
			pinMode(WARNLED_B_PIN, OUTPUT);
			#endif
			
			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.clear();
			oled.setFont(MAIN_FONT);
			//oled.print(F("CTAPT..."));	// no chars in font ((

			//temp = 0;

			//clocktick = true;
			/*gasH2S = 0;
			gasCH4 = 0;*/
			temp = 0;
			humidity = 0;
			pressure = 0;

			this->updateScreen = false;
		}

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

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new DisplayProcess(msg);
		}

		//@implement
		void update(unsigned long ms) {
			// oled contrast auto adjustment
			#ifdef PHOTORESISTOR_PIN
				oled.setContrast(100);
			#endif

			if (this->pressure > 0) {
				if (this->temp > 32 || this->temp < 20) {
					this->addWarning(1, SF("Temperature"), this->temp);
				} else {
					this->removeWarning(1);
				}

				if (this->humidity > 50 || this->humidity < 15) {
					this->addWarning(2, SF("Humidity"), this->humidity);
				} else {
					this->removeWarning(2);
				}
			}

			if (this->updateScreen) {
				this->render();
				this->updateScreen = false;
			}

			/*if (this->gasH2S > 0) {
				printGasInfo(SPRITE_GAS_H2S, SCREENROW_GAS_H2S, this->gasH2S);
			}
			if (this->gasCH4 > 0) {
				printGasInfo(SPRITE_GAS_CH4, SCREENROW_GAS_CH4, this->gasCH4);
			}*/

			this->pause(42);
		}

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

		//@implement
		//@include <SSD1306AsciiWire.h>
		void render() {
			oled.setCursor(0, 3);
			prn2X(this->time);

			if (this->humidity > 0) {
				oled.setCursor(95, 2);
				prn(String(round(this->temp)));
				prn(" c");
				
				oled.setCursor(95, 4);
				prn(String(round(this->humidity)));
				prn(" %");

				oled.setCursor(95, 6);
				prn(String(round(this->pressure)));
				prn("mm");
			}

			//oled.setCursor(0, 0);
			//prn(F("          "));
			//oled.clearField(0, 0, 19);
			for (uint16_t i = 0; i < this->warnings.size(); i++) {
				uint8_t pos = 120-i*1.4;
				if (pos < 1) {
					break;
				}
				oled.setCursor(pos, 0);
				prn(F("!"));
			} 
		}

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					this->handleEnvDataMsg((EnvDataMessage*)msg);
					break;
				}
				case AIRQUALITY_MESSAGE: {
					this->handleAirQualityMsg((AirQualityMsg*)msg);
					break;
				}
				case CURTIME_MESSAGE: {
					this->handleTimeMsg((CurrentTimeMsg*)msg);
					return true; // dispose
				}
			}
			return false;
		}

		//@implement
		void handleEnvDataMsg(EnvDataMessage* msg) {
			if (this->temp != msg->getTemp()) {
				this->temp = msg->getTemp();
				this->updateScreen = true;
			}

			if (this->humidity != msg->getHumidity()) {
				this->humidity = msg->getHumidity();
				this->updateScreen = true;
			}

			if (this->pressure != msg->getPressure()) {
				this->pressure = msg->getPressure();
				this->updateScreen = true;
			}
		}

		#if USE_WARNING_LIGHT == 1
		//@include "stuff.h"
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

		//@implement
		void handleTimeMsg(CurrentTimeMsg* msg) {
			//clocktick = !clocktick;
			//TRACELN(msg->getTime());

			this->time = msg->getTime();
			this->updateScreen = true;

			/*oled.setFont(MAIN_FONT);
			oled.setCursor(0, 1);
			prn2X(msg->getTime());
			oled.print(msg->getTime());
			if (!msg->getDots()) {
				oled.clearField(15, 0, 1);
			}*/
			//oled.set2X();
			//oled.clearField(0, 0, 5);
			//oled.print(msg->getTime());
			//oled.set1X();

			#if USE_WARNING_LIGHT == 1
				this->updateWarningLight();
			#endif
		}

		void prn(String str) {
			oled.print(str);
		}

		//@implement
		void handleAirQualityMsg(AirQualityMsg* msg) {
			switch (msg->gasType())
			{
				/*case H2S: {
					if (this->gasH2S != msg->getQuality()) {
						this->gasH2S = msg->getQuality();
						printGasInfo(SPRITE_GAS_H2S, SCREENROW_GAS_H2S, this->gasH2S);
					}
					return;
				}
				case CH4: {
					if (this->gasCH4 != msg->getQuality()) {
						this->gasCH4 = msg->getQuality();
						printGasInfo(SPRITE_GAS_CH4, SCREENROW_GAS_CH4, this->gasCH4);
					}
					return;
				}*/
				/*case CO2: {
					if (this->gasCO2 != msg->getQuality()) {
						this->gasCO2 = msg->getQuality();
						printGasInfo(SPRITE_GAS_CO2, SCREENROW_GAS_CO2, this->gasCO2);
					}
					return;
				}*/
			}
		}
};

#endif
