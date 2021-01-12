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
		bool updateScreen;

		float	temp;
		byte	humidity;
		int		pressure;
		float	altitude;  //int
	public:
		//@implement
		//@include <SSD1306AsciiWire.h>
		MainProcess(String & id, IProcessMessage* msg): IFirmwareProcess(id, msg) {
			TRACELNF("MainProcess::start");
			
			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.setFont(System5x7);
			oled.clear();
			
			this->updateScreen = false;
		}

		static IFirmwareProcess* factory(String & name, IProcessMessage* msg) {
			TRACELNF("MainProcess::factory");
			return new MainProcess(name, msg);
		}

		//@implement
		~MainProcess() {
			// stop process
			TRACELNF("MainProcess::stop");
			//delete this->display;
		}

		//@implement
		//@include <SSD1306AsciiWire.h>
		void update(unsigned long ms) {
			//this->log("MainProcess::run");
			if (this->updateScreen) {
				this->render();
				this->updateScreen = false;
			}
		}

		void render() {
			oled.clear();
			//oled.print(F("Hello world!"));
			oled.setCursor(10, 2);
			oled.print(F("T="));
			oled.print(this->temp);
			oled.print(F("C"));
			oled.setCursor(10, 3);
			oled.print(F("H="));
			oled.print(this->humidity);
			oled.print(F("%"));
			oled.setCursor(10, 4);
			oled.print(F("P="));
			oled.print(this->pressure);
			oled.print(F("mm"));
		}

		//@implement
		//@include "stuff.h"
		bool handleMessage(IProcessMessage* msg) {
			if (msg->getType() == ENVDATA_MESSAGE) {
				this->handleEnvDataMsg((EnvDataMessage*)msg);
				return true;
			}
			return false;
		}

		void handleEnvDataMsg(EnvDataMessage* msg) {
			if (msg->isActive()) {
				String s = SF("BME data: T=");
				s += msg->getTemp();
				s += F("C, Hum=");
				s += msg->getHumidity();
				s += F("%, Pres=");
				s += msg->getPressure();
				s += F("mm, Alt=");
				s += msg->getAltitude();
				s += F("m");
				TRACELN(s);
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
				if (this->altitude != msg->getAltitude()) {
					this->altitude = msg->getAltitude();
					this->updateScreen = true;
				}
			} else {
				TRACELNF("BME Sensors is passive!");
			}
		}
};

#endif
