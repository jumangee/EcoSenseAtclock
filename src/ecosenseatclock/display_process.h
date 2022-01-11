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

#undef OPTIMIZE_I2C
#define OPTIMIZE_I2C 0
#undef INCLUDE_SCROLLING
#define INCLUDE_SCROLLING 0

#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#include <math.h>

struct WarningInfo {
	uint8_t id;
	float value;
};

class DisplayProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire	oled;
		bool				updateScreen = false;
		bool				updateWarnings = false;

		float				temp = 0;
		float				humidity = 0;
		uint16_t			pressure = 0;
		uint8_t				timeH = 0;
		uint8_t				timeM = 0;
		bool				timeDots = true;

		WarningInfo*		warnings[MAX_DISPLAY_WARNINGS];
		uint8_t				warningsCount = 0;
		int					showWarningNum = -1;
		bool				online = false;

	public:
		PROCESSID(PRC_DISPLAY);
	
		//@implement
		//@include <SSD1306AsciiWire.h>
		DisplayProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			for (uint8_t i = 0; i < MAX_DISPLAY_WARNINGS; i++) {
				this->warnings[i] = NULL;
			}

			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.clear();
			oled.setFont(MAIN_FONT);
		}

		//@implement
		void addWarning(uint32_t id, float value) {
			int warnPos = this->findWarning(id);
			if (warnPos > -1) {
				this->warnings[warnPos]->value = value;
				return;
			}
			if (warningsCount >= MAX_DISPLAY_WARNINGS) {
				return;
			}

			for (warnPos = 0; warnPos < MAX_DISPLAY_WARNINGS; warnPos++) {
				if (this->warnings[warnPos] == NULL) {
					WarningInfo* warning = new WarningInfo();
					warning->id = id;
					warning->value = value;
					warningsCount++;

					this->warnings[warnPos] = warning;
					updateWarnings = true;
					return;
				}
			} 
		}

		//@implement
		int findWarning(uint32_t id) {
			for (uint8_t i = 0; i < MAX_DISPLAY_WARNINGS; i++) {
				if (this->warnings[i] != NULL && this->warnings[i]->id == id) {
					return i;
				}
			}
			return -1;
		}

		//@implement
		void removeWarning(uint32_t id) {
			int warnPos = this->findWarning(id);
			if (warnPos == -1) {
				return;
			}
			if (this->warnings[warnPos] != NULL) {
				delete this->warnings[warnPos];
				this->warnings[warnPos] = NULL;
				warningsCount -= warningsCount > 0 ? 1 : 0;
				updateWarnings = true;
			}
		}

		//@implement
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new DisplayProcess(msg);
		}

		//@implement
		void update(unsigned long ms) {
			if (this->pressure > 0) {
				int warnPos = this->findWarning(1);
				if (this->temp > 32 || this->temp < 20) {
					if (warnPos == -1)
						this->addWarning(1, this->temp);
				} else if (warnPos > -1) {
					this->removeWarning(1);
				}

				warnPos = this->findWarning(2);
				if (this->humidity > 50 || this->humidity < 15) {
					if (warnPos == -1)
						this->addWarning(2, this->humidity);
				} else if (warnPos > -1) {
					this->removeWarning(2);
				}
			}

			if (showWarningNum >= warningsCount) {
				showWarningNum = -1;
				oled.clear();
				updateScreen = true;
				updateWarnings = true;
			}

			if (this->updateScreen) {
				this->render();
			}

			if (this->updateWarnings) {
				this->renderWarnings();
			}

			this->pause(42);
		}

		//@implement
		void renderMainScreen() {
			// time

			oled.setCursor(0, 2);
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

			oled.setCursor(0, 4);
			char c = online ? '-' : '.';
			for (byte i = 0; i < 14; i++) {
				oled.print(c);
			}

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
			
		}

		//@implement
		const __FlashStringHelper* getTitle(uint8_t code) {
			switch (code) {
				case 1: return	F("TEMPERATURE");
				case 2: return	F("HUMIDITY");
				case 10: return F("GAS");
				case 11: return F("H2S");
				case 12: return F("CO");
				case 14: return F("CO2");
				case 15: return F("CH4");
				//case 18: return F("PM1");
				case 19: return F("PM25");
				case 20: return F("VOCs");
			}
		}

		//@implement
		int warnNumToPos(uint8_t num) {
			uint8_t cur = 0;
			for (uint8_t i = 0; i < warningsCount; i++) {
				if (this->warnings[i] != NULL) {
					if (cur == num) {
						return i;
					}
					cur++;
				}
			}
			return -1;
		}

		//@implement
		void renderWarningScreen() {
			oled.set2X();
			uint8_t warnPos = warnNumToPos(this->showWarningNum);
			if (warnPos == -1) {
				return;
			}
			WarningInfo* warn = this->warnings[warnPos];
			oled.setCursor(0, 5);
			oled.clearToEOL();
			showEvent(0, 2, getTitle(warn->id));
			oled.setCursor(0, 5);
			oled.print(warn->value);
			oled.set1X();
		}

		//@implement
		//@include <SSD1306AsciiWire.h>
		void render() {
			if (showWarningNum == -1) {
				renderMainScreen();
			}
			else {	// WARNINGS MODE
				renderWarningScreen();
			}			

			this->updateScreen = false;
		}

		//@implement
		void renderWarnings() {
			oled.setCursor(0, 0);
			oled.set2X();
			oled.clearToEOL();
			uint16_t i;
			for (i = 0; i < warningsCount; i++) {
				uint8_t pos = 118-i*10;
				if (pos < 1) {
					break;
				}
				oled.setCursor(pos, 0);
				if (i == this->showWarningNum) {
					oled.setInvertMode(true);
				}
				oled.print(F("!"));
				if (i == this->showWarningNum) {
					oled.setInvertMode(false);
				}
			}
			if (this->showWarningNum > -1) {
				oled.setCursor(118-i*10, 0);
				oled.print(F("*"));
			}
			oled.set1X();

			this->updateWarnings = false;
		}

		//@implement
		//@include "ecosense_messages.h"
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					this->handleEnvDataMsg((EnvDataMessage*)msg);
					return false;
				}
				case AIRQUALITY_MESSAGE: {
					this->handleAirQualityMsg((AirQualityMsg*)msg);
					return false;
				}
				case CURTIME_MESSAGE: {
					this->handleTimeMsg((CurrentTimeMsg*)msg);
					return false;
				}
				case BTNCLICK_MESSAGE: {	// this is always a click
					if (warningsCount > 0) {
						if (this->showWarningNum == -1) {
							oled.clear();
						}
						this->showWarningNum++;
						this->updateWarnings = true;
						this->updateScreen = true;
					}
					return true; // dispose
				}
				/*case WIFISTATE_MESSAGE: {
					this->online = ((WifiStateMessage*)msg)->online;
					return false;
				}*/
			}
			return false;
		}

		//@implement
		void showEvent(uint8_t x, uint8_t y, const __FlashStringHelper *pstr) {
			oled.setCursor(x, y);
			oled.print(pstr);
		}

		//@implement
		void handleEnvDataMsg(EnvDataMessage* msg) {
			this->temp = msg->temp;
			this->humidity = msg->humidity;
			this->pressure = msg->pressure;
			this->updateScreen = true;
		}

		//@implement
		void handleTimeMsg(CurrentTimeMsg* msg) {
			this->timeH	= msg->getHrs();
			this->timeM	= msg->getMins();
			this->timeDots = !this->timeDots;
			this->updateScreen = true;
		}

		//@implement
		//@include "ecosense_messages.h"
		void handleAirQualityMsg(AirQualityMsg* msg) {
			AirQualityMsg::GasType gas = msg->gasType();
			uint16_t gasCode = static_cast<uint16_t>(gas) + 10;
			uint16_t gasLevel = static_cast<uint16_t>(msg->getConcentration());
			if (gasLevel > 1) {
				this->addWarning(gasCode, msg->getAmount());
			} else {
				this->removeWarning(gasCode);
			}

			if (gas == AirQualityMsg::GasType::CO2 && showWarningNum == -1) {
				showEvent(0, 6, getTitle(gasCode));
				oled.print(F(": "));
				oled.print(round(msg->getAmount()));
				oled.print(F("   "));
			}
		}
};

#endif
