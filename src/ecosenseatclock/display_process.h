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
	
		//@implement
		//@include <SSD1306AsciiWire.h>
		DisplayProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.clear();
			oled.setFont(MAIN_FONT);

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

			if (this->updateScreen) {
				this->render();
				this->updateScreen = false;
			}

			this->pause(42);
		}


		//@implement
		void renderMainScreen() {
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
		}

		//@implement
		void renderWarningScreen() {
			oled.set2X();
			oled.setCursor(0, 2);
			WarningInfo* warn = this->warnings.get(this->showWarningNum);
			switch (warn->id) {
				case 1: oled.print(F("TEMPERATURE")); break;
				case 2: oled.print(F("HUMIDITY")); break;
				case 10: oled.print(F("AIR QUALITY")); break;
				case 11: oled.print(F("H2S")); break;
				case 12: oled.print(F("CO")); break;
				case 13: oled.print(F("SO2")); break;
				case 14: oled.print(F("CO2")); break;
				case 15: oled.print(F("CH4")); break;
				//case 16: oled.print(F("CH2O")); break;
				//case 17: oled.print(F("C6H5_CH3")); break;
				case 18: oled.print(F("PM1")); break;
				case 19: oled.print(F("PM25")); break;
				case 20: oled.print(F("VOCs")); break;
				/*default: {
					oled.print(warn->id);
				}*/
			}
			oled.setCursor(0, 4);
			oled.print(warn->value);
			oled.set1X();
		}

		//@implement
		//@include <SSD1306AsciiWire.h>
		void render() {
			if (showWarningNum > this->warnings.size()-1) {
				showWarningNum = -1;
			}

			if (showWarningNum == -1) {
				renderMainScreen();
			}
			else {	// WARNINGS MODE
				renderWarningScreen();
			}

			// warnings

			oled.setCursor(0, 0);
			oled.set2X();
			oled.clearToEOL();
			//oled.setInvertMode(true);
			uint16_t i;
			for (i = 0; i < this->warnings.size(); i++) {
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
				oled.setCursor(118-(i+1)*10, 0);
				oled.print(F("*"));
			}
			oled.set1X();
		}

		//@implement
		//@include "ecosense_messages.h"
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
				case BTNCLICK_MESSAGE: {
					oled.clear();
					this->showWarningNum++;
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
			uint16_t gasCode = static_cast<uint16_t>(msg->gasType()) + 10;
			uint16_t gasLevel = static_cast<uint16_t>(msg->getConcentration());
			if (gasLevel > 1) {
				this->addWarning(gasCode, msg->getAmount());
			} else {
				this->removeWarning(gasCode);
			}
		}
};

#endif
