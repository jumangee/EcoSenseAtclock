/**
 * WiFi connectivity and ThingSpeak report process
 * for Processy Firmware Framework
 */

#ifndef _METEOCLOCK_WIFI_H
#define _METEOCLOCK_WIFI_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"

//#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EspDrv/EspDrv.h"

#define _ESPLOGLEVEL_ 0

#include "meteo_messages.h"
#include "url_helper.h"

#define TIMEOUT 5000 // mS

#define RX_PIN 2
#define TX_PIN 3

#define REPORT_TIMEOUT 60000

#define THINGSPEAK_CHANNEL_KEY "43RGUGMOBYBHCJV2"
#define WIFI_SSID "Jumangee"
#define WIFI_PWD "54d75bc245"

class WifiProcess: public IFirmwareProcess {
	private:
		bool ready;
		SoftwareSerial *espSerial;
		unsigned long lastReportTime;
		ThingspeakWebSendTask dataSendTask;
	public:
		WifiProcess(int pId, IProcessMessage* msg);

		static IFirmwareProcess* factory(int pId, IProcessMessage* msg);

		~WifiProcess();

		void update(unsigned long ms);

		bool WiFiConnect() {
			if (this->ready) {
				// attempt to connect to WiFi network
				this->getHost()->sendMessage(new WiFiStateMsg(true));
				if (EspDrv::getConnectionStatus() != WL_CONNECTED) {
					TRACELNF("[WIFI] Attempting to connect");
					if (EspDrv::wifiConnect(SF(WIFI_SSID).c_str(), SF(WIFI_PWD).c_str())) {
						TRACELNF("[WIFI] Connected to the network");
						this->dataSendTask.setParam(7, uint16_t(EspDrv::getCurrentRSSI() / 1000));
						return true;
					} else {
						TRACELNF("[WIFI] Connection error");
					}
				} else {
					return true;
				}
			}
			this->getHost()->sendMessage(new WiFiStateMsg(false));
			return false;
		}

		void WiFiDisconnect() {
			EspDrv::disconnect();
			this->getHost()->sendMessage(new WiFiStateMsg(false));
		}

		void simpleSendData() {
			uint8_t _sock = 1;
			String server = dataSendTask.getUrl(F(THINGSPEAK_CHANNEL_KEY));
			if (EspDrv::startClient(server.c_str(), 80, _sock, TCP_MODE)) {
				TRACELNF("Connected to server");
				{
					String buf = SF("GET ");
					buf += server;
					buf += F(" HTTP/1.1\r\nHost: ");
					buf += dataSendTask.getServer(); 
					buf += F("\r\nConnection: close\r\n\r\n");
					TRACELN(buf);
					EspDrv::sendData(_sock, buf.c_str(), buf.length());
					/*if (!EspDrv::sendData(_sock, buf.c_str(), buf.length())) {
						TRACELNF("[simpleSendData] Error");
					}*/
				}
				EspDrv::stopClient(_sock);
			}
		}

		/*//@implement
		//!@include "meteo_messages.h"
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
