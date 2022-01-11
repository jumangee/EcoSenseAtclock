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
	
		DisplayProcess(IProcessMessage* msg);

		void addWarning(uint32_t id, float value);

		int findWarning(uint32_t id);

		void removeWarning(uint32_t id);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		void update(unsigned long ms);

		void renderMainScreen();

		const __FlashStringHelper* getTitle(uint8_t code);

		int warnNumToPos(uint8_t num);

		void renderWarningScreen();

		void render();

		void renderWarnings();

		bool handleMessage(IProcessMessage* msg);

		void showEvent(uint8_t x, uint8_t y, const __FlashStringHelper *pstr);

		void handleEnvDataMsg(EnvDataMessage* msg);

		void handleTimeMsg(CurrentTimeMsg* msg);

		void handleAirQualityMsg(AirQualityMsg* msg);
};

#endif
