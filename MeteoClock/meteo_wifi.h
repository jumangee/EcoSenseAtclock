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
		WifiProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		~WifiProcess();

		void update(unsigned long ms);

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
