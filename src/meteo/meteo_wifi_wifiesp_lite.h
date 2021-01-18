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

#include "meteo_cfg.h"
#include "meteo_messages.h"
#include "url_helper.h"

#define TIMEOUT 5000 // mS

#define RX_PIN 2
#define TX_PIN 3


class WifiProcess: public IFirmwareProcess {
	private:
		bool ready;
		SoftwareSerial *espSerial;
		unsigned long lastReportTime;
		ThingspeakWebSendTask dataSendTask;
	public:
		//@implement
		//@include "meteo_cfg.h"
		//@include "EspDrv/RingBuffer.cpp"
		//@include "EspDrv/EspDrv.cpp"
		WifiProcess(int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			//this->log("WifiProcess::start");

			TRACELNF("WifiProcess::init");

			pinMode(RX_PIN, INPUT);
  			pinMode(TX_PIN, OUTPUT);
			lastReportTime = 0;
			ready = false;

			espSerial = new SoftwareSerial(RX_PIN, TX_PIN); // RX, TX
			espSerial->begin(19200);

			TRACELN("Initializing ESP module...")
			EspDrv::wifiDriverInit(espSerial); 

			// check for the presence of the shield
			if (EspDrv::getConnectionStatus() == WL_NO_SHIELD) {
				TRACELNF("[WiFi] FAIL");
				delete this->espSerial;
				this->espSerial = NULL;
			} else {
				TRACELNF("[WiFi] OK");
				this->ready = true;
			}
			this->pause(ENVSENSORS_TIMEOUT);
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
			if (espSerial != NULL) {
				delete espSerial;
			}
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "meteo_cfg.h"
		//@include "EspDrv/EspDrv.h"
		void update(unsigned long ms) {
			if (!this->ready) return;
			
			unsigned long now = millis();
			
			if (this->dataSendTask.size() && now - lastReportTime > REPORT_TIMEOUT) {
				if (WiFiConnect()) {
					simpleSendData();
					dataSendTask.clear();
					WiFiDisconnect();
					TRACELN("[WIFI] Send packets done")

					lastReportTime = millis();
				} else {
					//TRACELNF("[WIFI] Failed to clearing tasks buf!")
					dataSendTask.clear();
				}
			}

			this->pause(ENVSENSORS_TIMEOUT);
		}

		//@implement
		bool WiFiConnect() {
			if (this->ready) {
				// attempt to connect to WiFi network
				this->getHost()->sendMessage(new WiFiStateMsg(true));
				if (EspDrv::getConnectionStatus() != WL_CONNECTED) {
					TRACELNF("[WIFI] Attempting to connect");
					if (EspDrv::wifiConnect(SF(WIFI_SSID).c_str(), SF(WIFI_PWD).c_str())) {
						//TRACELNF("[WIFI] Connected to the network");
						this->dataSendTask.setParam(7, uint16_t(abs(EspDrv::getCurrentRSSI())));	//byte((double(EspDrv::getCurrentRSSI()) / 65535.0*100.0))
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

		//@implement
		void WiFiDisconnect() {
			EspDrv::disconnect();
			this->getHost()->sendMessage(new WiFiStateMsg(false));
		}

		//@implement
		void simpleSendData() {
			uint8_t _sock = 1;
			String server = dataSendTask.getServer();
			if (EspDrv::startClient(server.c_str(), 80, _sock, TCP_MODE)) {
				TRACEF("Connected to server ");
				TRACELN(server);
				{
					String buf = SF("GET ");
					buf += dataSendTask.getUrl(F(THINGSPEAK_CHANNEL_KEY)); 
					buf += F(" HTTP/1.1\r\nHost: ");
					buf += server;
					buf += F("\r\nConnection: close\r\n\r\n");
					TRACELN(buf);
					if (!EspDrv::sendData(_sock, buf.c_str(), buf.length())) {
						TRACELNF("[simpleSendData] Error");
					}
				}
				EspDrv::stopClient(_sock);
			}
		}

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

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			if (!this->ready) return false;

			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					EnvDataMessage* env = (EnvDataMessage*)msg;
					if (env->isActive()) {
						this->dataSendTask.setParam(1, env->getTempF());
					}
					break;
				}
				case FREEMEM_MESSAGE: {
					this->dataSendTask.setParam(6, ((MemUsageMessage*)msg)->getFreemem());
					break;
				}
				case AIRQUALITY_MESSAGE: {
					this->handleAirQualityMsg((AirQualityMsg*)msg);
					break;
				}
			}
			return false;
		}

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
