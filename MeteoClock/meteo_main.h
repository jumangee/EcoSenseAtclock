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

#define OLED_ADDR   0x3C

//class Adafruit_SSD1306;

class MainProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire oled;
	public:
		MainProcess(String id, IProcessMessage* msg);

		~MainProcess();

		void update(unsigned long ms);

		bool handleMessage(IProcessMessage* msg);
};

#endif
