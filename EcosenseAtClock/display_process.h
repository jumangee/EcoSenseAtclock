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
		bool				wifiOn = true;

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

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);

		void renderMainScreen();

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

		void renderWarningScreen();

		void render();

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

		bool handleMessage(IProcessMessage* msg);

		void showEvent(uint8_t x, uint8_t y, const __FlashStringHelper *pstr) {
			oled.setCursor(x, y);
			oled.print(pstr);
		}

		void handleEnvDataMsg(EnvDataMessage* msg);

		void handleTimeMsg(CurrentTimeMsg* msg);

		void handleAirQualityMsg(AirQualityMsg* msg);
};

#endif
