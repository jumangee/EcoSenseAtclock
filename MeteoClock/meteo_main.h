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
		MainProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		~MainProcess();

		void update(unsigned long ms);

		void render();

		bool handleMessage(IProcessMessage* msg);

		void handleEnvDataMsg(EnvDataMessage* msg);

		void handleTimeMsg(CurrentTimeMsg* msg);

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
