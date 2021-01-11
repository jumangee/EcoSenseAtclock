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
		//@implement
		//@include <SSD1306AsciiWire.h>
		MainProcess(String id, IProcessMessage* msg): IFirmwareProcess(id, msg) {
			this->log("MainProcess::start");
			
			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.setFont(System5x7);

			this->getHost()->addProcess(PRC_SENSORS);
		}

		//@implement
		~MainProcess() {
			// stop process
			this->log("MainProcess::stop");
			//delete this->display;
		}

		//@implement
		//@include <SSD1306AsciiWire.h>
		void update(unsigned long ms) {
			//this->log("MainProcess::run");
			oled.clear();
			oled.print("Hello world!");

			this->pause(5000);
		}

		//@implement
		//@include "stuff.h"
		bool handleMessage(IProcessMessage* msg) {
			/*if (msg->getType() == MSG_ENV_DATA) {
				EnvDataMessage* msg = (EnvDataMessage*)msg;
				if (msg->isActive()) {
					this->log(S("BME data: T=", String(msg->getTemp()).c_str(),
					"C, Hum=", String(msg->getHumidity()).c_str(), "%, Pres=", 
					String(msg->getPressure()).c_str(), "mm, Alt=",
					String(msg->getAltitude()).c_str(), "m"));
				} else {
					this->log("BME Sensors is passive!");
				}
			}*/
			return false;
		}
};

#endif
