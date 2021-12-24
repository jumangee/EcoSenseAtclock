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

#include "LinkedList/LinkedList.h"

struct WarningInfo {
	uint16_t id;
	float value;
};

class DisplayProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire	oled;
		bool				updateScreen = false;
		bool				updateWarnings = false;

		float				temp = 0;
		float				humidity = 0;
		//float				co2 = 0;
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

			updateWarnings = true;
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
			updateWarnings = true;
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

			if (showWarningNum > this->warnings.size()-1) {
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

			showEvent(0, 4, F(".............."));

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

			/*IFirmware* f = this->getHost();
			if (f->getProcess(PRC_CONSUMER1)) {
				showEvent(0, 5, F("PWR: 1"));
			} else if (f->getProcess(PRC_CONSUMER2)) {
				showEvent(0, 5, F("PWR: 2"));
			} else if (f->getProcess(PRC_CONSUMER3)) {
				showEvent(0, 5, F("PWR: 3"));
			} else {
				showEvent(0, 5, F("PWR: -"));
			}*/

			/*if (this->co2 > 0) {
				oled.setCursor(95, 7);
				oled.print((uint16_t)co2);
				oled.print(F("co2"));
				oled.print(F(" "));
			}*/
		}

		const __FlashStringHelper* getTitle(uint16_t code) {
			switch (code) {
				case 1: return F("TEMPERATURE");
				case 2: return F("HUMIDITY");
				case 10: return F("GASes");
				case 11: return F("H2S");
				case 12: return F("CO");
				//case 13: return F("SO2");
				case 14: return F("CO2");
				case 15: return F("CH4");
				//case 16: return F("CH2O");
				//case 17: return F("C6H5_CH3");
				//case 18: return F("PM1");
				case 19: return F("PM25");
				case 20: return F("VOCs");
			}
		}

		//@implement
		void renderWarningScreen() {
			oled.set2X();
			//oled.setCursor(0, 2);
			WarningInfo* warn = this->warnings.get(this->showWarningNum);
			//oled.print(getTitle(warn->id));
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

		void renderWarnings() {
			oled.setCursor(0, 0);
			oled.set2X();
			oled.clearToEOL();
			//oled.print(F("                  "));
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
				oled.setCursor(118-i*10, 0);
				oled.print(F("*"));
			}
			oled.set1X();

			this->updateWarnings = false;
		}

		//@implement
		//@include "ecosense_messages.h"
		bool handleMessage(IProcessMessage* msg) {
			//oled.setCursor(0, 6);

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
				case BTNCLICK_MESSAGE: {
					if (warnings.size() > 0) {
						if (this->showWarningNum == -1) {
							oled.clear();
						}
						this->showWarningNum++;
						this->updateWarnings = true;
						this->updateScreen = true;
					}
					return true; // dispose
				}
				/*case TASKDONE_MESSAGE: {
					TaskDoneMessage* e = (TaskDoneMessage*)msg;
					showEvent(0, 7, F("DONE: "));
					oled.print(e->getTaskId());
					return false;
				}*/
				case WIFIEVENT_MESSAGE: {
					WifiEventMessage* e = (WifiEventMessage*)msg;
					if (e->event == WifiEventMessage::WifiEvent::ERROR) {
						showEvent(0, 7, F("WIFI: ERR "));
					}
					else if (e->event == WifiEventMessage::WifiEvent::NONE) {
						showEvent(0, 7, F("WIFI: OK  "));
					}
					return false;
				}
			}
			return false;
		}

		void showEvent(uint8_t x, uint8_t y, const __FlashStringHelper *pstr) {
			oled.setCursor(x, y);
			oled.print(pstr);
		}

		//@implement
		void handleEnvDataMsg(EnvDataMessage* msg) {
			//if (this->temp != msg->getTemp()) {
				this->temp = msg->getTemp();
			//	this->updateScreen = true;
			//}

			//if (this->humidity != msg->getHumidity()) {
				this->humidity = msg->getHumidity();
			//	this->updateScreen = true;
			//}

			//if (this->pressure != msg->getPressure()) {
				this->pressure = msg->getPressure();
				this->updateScreen = true;
			//}
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

			/*printTitle(gasCode);
			oled.print(F(": "));
			oled.print(round(msg->getAmount()));
			oled.print(F("   "));*/

			if (gas == AirQualityMsg::GasType::CO2) {
				showEvent(0, 6, getTitle(gasCode));
				oled.print(F(": "));
				oled.print(round(msg->getAmount()));
				oled.print(F("   "));
			}
		}
};

#endif
