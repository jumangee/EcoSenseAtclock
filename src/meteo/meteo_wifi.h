/*
* MeteClock Firmware
*/

#ifndef _METEOCLOCK_WIFI_H
#define _METEOCLOCK_WIFI_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"

//#include <Arduino.h>
//#include <.h>
#define TIMEOUT 5000 // mS

class WifiProcess: public IFirmwareProcess {
	private:
		bool ready;
		//SoftwareSerial wifiSerial;
	public:
		//@implement
		//@include <Adafruit_BME280.h>
		//@include "meteo_cfg.h"
		WifiProcess(int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			//this->log("WifiProcess::start");

			TRACELNF("WifiProcess::init");


			//this->sendCommand(F("AT+RST"), F("Ready"));
			this->ready = false;
			this->pause(5000);
		}

		//@implement
		//@include <Arduino.h>
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("WifiProcess::factory");
			return new WifiProcess(pId, msg);
		}

		//@implement
		//@include <Arduino.h>
		~WifiProcess() {
			// stop process
			TRACELNF("WifiProcess::stop");
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "meteo_cfg.h"
		//@include "stuff.h"
		void update(unsigned long ms) {
			/*TRACELNF("WifiProcess::run");
			if (!this->ready) {
				this->sendCommand("AT+CWMODE=1","OK");
				this->sendCommand("AT+CIFSR", "OK");
				this->ready = true;
			}*/
			
			this->pause(ENVSENSORS_TIMEOUT);
		}

		/*boolean sendCommand(String cmd, String ack){
			Serial.println(SF(F("Wifi cmd: ") + String(cmd)));
			wifiSerial.println(cmd); // Send "AT+" command to module
			if (this->echoFind(ack)) {
				return true; // ack blank or ack found
			}
		}

		boolean echoFind(String keyword){
			byte current_char = 0;
			byte keyword_length = keyword.length();
			long deadline = millis() + TIMEOUT;
			while(millis() < deadline) {
				if (mySerial.available()) {
					char ch = this->wifiSerial.read();
					Serial.write(ch);
					if (ch == keyword[current_char])
						if (++current_char == keyword_length) {
							Serial.println();
							deadline = millis() - deadline;
							Serial.println(SF("(response: ") + (deadline));
							return true;
						}
				}
			}
			return false; // Timed out
		}*/
};

#endif
