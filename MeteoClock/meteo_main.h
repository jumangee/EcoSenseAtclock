/*
* MeteClock Firmware MAIN process
*/

#ifndef _METEOCLOCK_MAIN_H
#define _METEOCLOCK_MAIN_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "meteo_process.h"
#include "meteo_messages.h"

#include <Arduino.h>

#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <math.h>
#include "meteoclockicons.h"

#define OLED_ADDR   0x3C
#define ANIM_WAIT_TIME 50

//class Adafruit_SSD1306;

class MainProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire oled;
		bool updateScreen;

		int		temp;
		byte	humidity;
		int		pressure;
		byte	freeMem;

		byte waitAnimPos;
		int animTimer;
	public:
		MainProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		~MainProcess();

		void update(unsigned long ms);

		void render();

		bool handleMessage(IProcessMessage* msg);

		void handleEnvDataMsg(EnvDataMessage* msg);

		void handleMemUsageMsg(MemUsageMessage* msg);

		void handleTimeMsg(CurrentTimeMsg* msg);

		void handleAirQualityMsg(AirQualityMsg* msg) {
			String gasInfo;
			gasInfo.reserve(1);
			oled.setFont(meteoclockicons);
			byte y;

			switch (msg->gasType())
			{
				case H2S: {
					gasInfo = F("R");
					y = 3;
					break;
				}
				case CH4: {
					gasInfo = F("P");
					y = 5;
					break;
				}
			}

			oled.setCursor(95, y);
			oled.print(gasInfo);

			if (msg->getQuality() == 0) {
				gasInfo = F("O");
			} else if (msg->getQuality() == 1) {
				gasInfo = F("!");
			} else
				gasInfo = F(" ");

			oled.setCursor(111, y);
			oled.print(gasInfo);
		}
};

#endif
