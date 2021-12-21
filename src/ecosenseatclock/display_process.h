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
	
		//@implement
		//@include <SSD1306AsciiWire.h>
		DisplayProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			TRACELNF("DisplayProcess::start");

			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.clear();
			oled.setFont(MAIN_FONT);

			timeDots = true;
			temp = 0;
			humidity = 0;
			pressure = 0;

			this->updateScreen = false;
		}

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

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new DisplayProcess(msg);
		}

		//@implement
		void update(unsigned long ms) {
			if (this->pressure > 0) {
				if (this->temp > 32 || this->temp < 20) {
					this->addWarning(1, this->temp);
				} else {
					this->removeWarning(1);
				}

				if (this->humidity > 50 || this->humidity < 15) {
					this->addWarning(2, this->humidity);
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

		//@implement
		//@include <SSD1306AsciiWire.h>
		void render() {

			// time

			oled.setCursor(0, 3);
			oled.set2X();
			if (this->timeH < 10) {
				oled.print(F("0"));
			}
			oled.print(this->timeH);
			oled.print(this->timeDots ? F(":") : F(" "));
			if (this->timeM < 10) {
				oled.print(F("0"));
			}
			oled.print(this->timeM);
			oled.set1X();

			// info

			if (this->humidity > 0) {
				oled.setCursor(95, 2);
				oled.print(round(this->temp));
				oled.print(F(" c"));
				
				oled.setCursor(95, 4);
				oled.print(round(this->humidity));
				oled.print(F(" %"));

				oled.setCursor(95, 6);
				oled.print(round(this->pressure));
				oled.print(F("mm"));
			}

			//oled.setCursor(0, 0);
			//prn(F("          "));

			// warnings

			oled.setCursor(0, 0);
			oled.set2X();
			oled.clearToEOL();
			//oled.setInvertMode(true);
			for (uint16_t i = 0; i < this->warnings.size(); i++) {
				uint8_t pos = 118-i*10;
				if (pos < 1) {
					break;
				}
				oled.setCursor(pos, 0);
				oled.print(F("!"));
			}
			oled.set1X();
			//oled.setInvertMode(false);
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

		//@implement
		void handleTimeMsg(CurrentTimeMsg* msg) {
			//TRACELN(msg->getTime());

			this->timeH	= msg->getHrs();
			this->timeM	= msg->getMins();
			this->timeDots = !this->timeDots;
			this->updateScreen = true;
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
