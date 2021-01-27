/**
 * WiFi connectivity and ThingSpeak report process
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_WIFI_H
#define _ECOSENSEATCLOCK_WIFI_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"

//#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EspDrv/EspDrv.h"

#define _ESPLOGLEVEL_ 0

#include "ecosense_cfg.h"
#include "ecosense_messages.h"
#include "url_helper.h"

//#define TIMEOUT 5000 // mS

class WifiProcess: public IFirmwareProcess {
	private:
		bool ready;
		SoftwareSerial *espSerial;
		unsigned long lastReportTime;
		ThingspeakWebSendTask dataSendTask;
	public:
		WifiProcess(uint16_t pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

		~WifiProcess();

		void update(unsigned long ms);

		bool WiFiConnect();

		void WiFiDisconnect();

		void simpleSendData();

		/*String receiveData() {
			String buf;
			bool connClose = false;
			while (!connClose) {
				int bytes = EspDrv::availData(_sock);
				buf.reserve(bytes);
				for (int i = 0; i < bytes; i++) {
					char c;
					EspDrv::getData(_sock, &c, false, &connClose);
					buf += c;
				}
			}
			return buf;
		}*/

		/*//@implement
		//!@include "ecosense_messages.h"
		//!@include "EspDrv/EspDrv.h"
		void sendTempData(byte temp) {
			unsigned long now = millis();
			if ( now - lastReportTime > REPORT_TIMEOUT) {
				TRACELNF("Time to report!");

				if (!WiFiConnect()) {
					return;
				}

				uint8_t _sock = 1;
				if (EspDrv::startClient(SFC("api.thingspeak.com"), 80, _sock, TCP_MODE)) {
					TRACELNF("Connected to server");
					{
						String data = SF("GET /update?api_key=43RGUGMOBYBHCJV2&field1=");
						data += temp;
						data += F(" HTTP/1.1\r\n");
						if (EspDrv::sendData(_sock, data.c_str(), data.length())) {
							data = SF("Host: api.thingspeak.com\r\n");
							if (EspDrv::sendData(_sock, data.c_str(), data.length())) {
								data = SF("Connection: close\r\n");
								if (EspDrv::sendData(_sock, data.c_str(), data.length())) {
									data = SF("\r\n");
									if (EspDrv::sendData(_sock, data.c_str(), data.length())) {
										TRACELNF("Request sent");
										delay(100);
									} else {
										TRACELNF("Error #3");
									}
								} else {
									TRACELNF("Error #3");
								}
							} else {
								TRACELNF("Error #2");
							}
						} else {
							TRACELNF("Error #1");
						}
					}
					EspDrv::stopClient(_sock);
				}

				WiFiDisconnect();

				lastReportTime = millis();
				TRACELNF("Report finished");
			}
			//GET https://api.thingspeak.com
		}*/

		bool handleMessage(IProcessMessage* msg);

		void handleAirQualityMsg(AirQualityMsg* msg) {
			switch (msg->gasType())
			{
				case H2S: {
					this->dataSendTask.setParam(2, msg->getVoltage());
					break;
				}
				case CH4: {
					this->dataSendTask.setParam(3, msg->getVoltage());
					break;
				}
			}
		}

};

#endif
