/**
 * Display process
 * for Processy Firmware Framework
 */

#ifndef _METEOCLOCK_MAIN_H
#define _METEOCLOCK_MAIN_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "meteo_messages.h"

#include <Arduino.h>

#define INCLUDE_SCROLLING 0

#include <SSD1306Ascii.h>

#undef OPTIMIZE_I2C
#define OPTIMIZE_I2C 0
#undef INCLUDE_SCROLLING
#define INCLUDE_SCROLLING 0

#include <SSD1306AsciiWire.h>


#include <math.h>
#include "meteoclockicons.h"

#define OLED_ADDR   0x3C

//#define MAIN_FONT System5x7
#define MAIN_FONT Stang5x7
#define ICONS_FONT meteoclockicons

#define SPRITE_GAS_CH4	0x2A
#define SPRITE_GAS_H2S	0x24
#define SPRITE_OK		0x2C
#define SPRITE_WARNING	0x21
#define SPRITE_DANGER	0x2E
#define SPRITE_SPC		0x20
#define SPRITE_WIFI		0x22


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
		//@implement
		//@include <SSD1306AsciiWire.h>
		MainProcess(int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			TRACELNF("MainProcess::start");
			
			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.clear();

			temp = 0;

			clocktick = true;
			gasH2S = 0;
			gasCH4 = 0;

			printGasInfo(SPRITE_GAS_H2S, 2, this->gasH2S);
			printGasInfo(SPRITE_GAS_CH4, 5, this->gasCH4);

			this->updateScreen = false;
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("MainProcess::factory");
			return new MainProcess(pId, msg);
		}

		//@implement
		~MainProcess() {
			// stop process
			TRACELNF("MainProcess::stop");
		}

		//@implement
		void update(unsigned long ms) {


			if (this->updateScreen) {
				this->render();
				this->updateScreen = false;
			}

			if (this->gasH2S > 0) {
				printGasInfo(SPRITE_GAS_H2S, 2, this->gasH2S);
			}
			if (this->gasCH4 > 0) {
				printGasInfo(SPRITE_GAS_CH4, 5, this->gasCH4);
			}

			this->pause(5);
		}

		//@implement
		//@include <SSD1306AsciiWire.h>
		void render() {
			oled.setFont(ICONS_FONT);
			oled.setCursor(0, 2);
			oled.print(F("( "));
			oled.setFont(MAIN_FONT);
			oled.set2X();
			oled.print(this->temp);
			oled.setFont(ICONS_FONT);
			oled.set1X();
			oled.print(F(")"));

			oled.setCursor(0, 4);
			oled.print(F("# "));
			if (this->humidity > 0) {
				oled.setFont(MAIN_FONT);
				oled.set2X();
				oled.print(this->humidity);
				oled.set1X();
				oled.setRow(oled.row()+1);
				oled.print(F("%"));
			} else {
				oled.setFont(MAIN_FONT);
				oled.set2X();
				oled.print(F("-   "));
				oled.set1X();
			}

			oled.setCursor(0, 6);			
			oled.setFont(ICONS_FONT);
			oled.print(F("- "));
			if (this->pressure > 0) {
				oled.setFont(MAIN_FONT);
				oled.set2X();
				oled.print(this->pressure);
				oled.setFont(ICONS_FONT);
				oled.set1X();
			} else {
				oled.setFont(MAIN_FONT);
				oled.set2X();
				oled.print(F("-   "));
				oled.set1X();
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
				case WIFISTATE_MESSAGE: {
					this->handleWifiMsg((WiFiStateMsg*)msg);
					return true; // dispose
				}
				
			}
			return false;
		}

		//@implement
		void handleEnvDataMsg(EnvDataMessage* msg) {
			if (msg->isActive()) {
				/*String s = SF("BME data: T=");
				s += msg->getTemp();
				s += F("C, Hum=");
				s += msg->getHumidity();
				s += F("%, Pres=");
				s += msg->getPressure();
				TRACELN(s);*/
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
			} else {
				TRACELNF("BME Sensors is passive!");
			}
		}

		//@implement
		void handleTimeMsg(CurrentTimeMsg* msg) {
			oled.setFont(Stang5x7);
			oled.setCursor(0, 0);
			oled.set2X();
			//oled.clearField(0, 0, 5);
			oled.print(msg->getTime());
			oled.set1X();

			clocktick = !clocktick;
		}

		void printGasInfo(char g, byte row, byte quality) {
			oled.setFont(ICONS_FONT);
			if (quality > 0) {
				oled.setInvertMode(clocktick);
			} else {
				oled.setInvertMode(false);
			}
			oled.setCursor(90, row);
			oled.print(g);
			oled.print(0x20);

			oled.setCol(108);
			if (quality == 0) {
				oled.print(SPRITE_OK);	//
			} else if (quality == 1) {
				oled.print(SPRITE_WARNING);
			} else {
				oled.print(SPRITE_DANGER);
			}
			oled.print(0x20);
			oled.setInvertMode(false);
		}

		void handleAirQualityMsg(AirQualityMsg* msg) {
			switch (msg->gasType())
			{
				case H2S: {
					if (this->gasH2S != msg->getQuality()) {
						this->gasH2S = msg->getQuality();
						printGasInfo(SPRITE_GAS_H2S, 2, this->gasH2S);
					}
					return;
				}
				case CH4: {
					if (this->gasCH4 != msg->getQuality()) {
						this->gasCH4 = msg->getQuality();
						printGasInfo(SPRITE_GAS_CH4, 5, this->gasCH4);
					}
					return;
				}
			}
		}

		/*void handleAirQualityMsg(AirQualityMsg* msg) {
			String gasInfo;
			gasInfo.reserve(2);
			oled.setFont(ICONS_FONT);
			byte y;

			if (msg->getQuality() > 0) {
				oled.setInvertMode(clocktick);
			}

			switch (msg->gasType())
			{
				case H2S: {
					gasInfo += F("R");
					y = 2;
					break;
				}
				case CH4: {
					gasInfo += F("P");
					y = 5;
					break;
				}
			}

			oled.setCursor(90, y);
			oled.print(gasInfo);

			if (msg->getQuality() == 0) {
				gasInfo = F("=");
			} else if (msg->getQuality() == 1) {
				gasInfo = F("!");
			} else {
				gasInfo = F(")");
			}
			oled.setCursor(111, y);
			oled.print(gasInfo);

			oled.setInvertMode(false);			
		}*/

		void handleWifiMsg(WiFiStateMsg* msg) {
			oled.setFont(ICONS_FONT);
			oled.setCursor(90, 0);
			if (msg->isActive()) {
				oled.print(SPRITE_WIFI);
			} else {
				oled.print(F("   "));
			}
		}
};

#endif
