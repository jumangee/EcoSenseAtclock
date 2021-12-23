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
#include "websend_helper.h"

#include "LinkedList/LinkedList.h"

//#define TIMEOUT 5000 // mS

class WifiProcess: public IFirmwareProcess {
	private:
		bool					ready;
		SoftwareSerial			*espSerial;
		unsigned long			lastReportTime;
		LinkedList<UrlParam*>	params;
	public:
		PROCESSID(PRC_WIFI);

		//@implement
		//@include "ecosense_cfg.h"
		//@include "EspDrv/RingBuffer.cpp"
		//@include "EspDrv/EspDrv.cpp"
		WifiProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			//this->log("WifiProcess::start");

			TRACELNF("WifiProcess::init");

			pinMode(WIFI_RX_PIN, INPUT);
  			pinMode(WIFI_TX_PIN, OUTPUT);
			lastReportTime = 0;
			ready = false;

			espSerial = new SoftwareSerial(WIFI_RX_PIN, WIFI_TX_PIN); // RX, TX
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
			this->pause(30000);
		}

		//@implement
		//@include <Arduino.h>
		static IFirmwareProcess* factory(IProcessMessage* msg) {
			return new WifiProcess(msg);
		}

		//@implement
		//@include <Arduino.h>
		~WifiProcess() {
			// stop process
			TRACELNF("WifiProcess::stop");
			clearParams();
			if (espSerial != NULL) {
				delete espSerial;
			}
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "ecosense_cfg.h"
		//@include "EspDrv/EspDrv.h"
		void update(unsigned long ms) {
			if (!this->ready) return;
			
			unsigned long now = millis();
			
			if (this->params.size() && now - lastReportTime > REPORT_TIMEOUT) {
				if (WiFiConnect()) {
					sendThingSpeak();
					WiFiDisconnect();
					TRACELN("[WIFI] Send packets done")

					lastReportTime = millis();
				} else {
					TRACELNF("[WIFI] Failed to clearing tasks buf!")
					//clearParams();
				}
			}

			this->pause(15000);
		}

		//@implement
		bool WiFiConnect() {
			if (this->ready) {
				// attempt to connect to WiFi network
				//this->getHost()->sendMessage(new WiFiStateMsg(true));
				if (EspDrv::getConnectionStatus() != WL_CONNECTED) {
					TRACELNF("[WIFI] Attempting to connect");
					if (EspDrv::wifiConnect(SF(WIFI_SSID).c_str(), SF(WIFI_PWD).c_str())) {
						//TRACELNF("[WIFI] Connected to the network");
						//this->dataSendTask.setParam(7, uint16_t(abs(EspDrv::getCurrentRSSI())));	//byte((double(EspDrv::getCurrentRSSI()) / 65535.0*100.0))
						return true;
					} else {
						TRACELNF("[WIFI] Connection error");
					}
				} else {
					return true;
				}
			}
			//this->getHost()->sendMessage(new WiFiStateMsg(false));
			return false;
		}

		//@implement
		void WiFiDisconnect() {
			EspDrv::disconnect();
			//this->getHost()->sendMessage(new WiFiStateMsg(false));
		}

		/*//@implement
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
		}*/

		void sendThingSpeak() {
			uint8_t _sock = 1;
			if (EspDrv::startClient(String(THINGSPEAK_SERVER).c_str(), 80, _sock, TCP_MODE)) {
				TRACEF("Connected to server ");
				TRACELN(THINGSPEAK_SERVER);
				{
					String buf = SF("GET ");
					buf.reserve(45);
					//buf += dataSendTask.getUrl(F(THINGSPEAK_CHANNEL_KEY)); 

					buf += F("/update?api_key=");
					buf += F(THINGSPEAK_CHANNEL_KEY);
					for (byte i = 0; i < this->params.size(); i++) {
						UrlParam* p = this->params.get(i);

						buf += F("&field");
						buf += p->getId();
						buf += F("=");
						buf += p->getValue().substring(0, 5);

						//buf += paramStr;
					}

					buf += F(" HTTP/1.1\r\nHost: ");
					buf += THINGSPEAK_SERVER;
					buf += F("\r\nConnection: close\r\n\r\n");
					
					TRACELN(buf);

					if (!EspDrv::sendData(_sock, buf.c_str(), buf.length())) {
						TRACELNF("[simpleSendData] Error");
					}
				}
				EspDrv::stopClient(_sock);
			}
		}

		//@implement
		//@include "ecosense_messages.h"
		bool handleMessage(IProcessMessage* msg) {
			if (!this->ready) return false;

			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					EnvDataMessage* env = (EnvDataMessage*)msg;
					//this->dataSendTask.setParam(THINGSPEAK_PARAM_TEMP, env->getTemp());
					
					UrlParam* param = new UrlParam();
					param->setValue(THINGSPEAK_PARAM_TEMP, env->getTemp());
					this->setParam(param);

					param = new UrlParam();
					param->setValue(THINGSPEAK_PARAM_HUMIDITY, env->getHumidity());
					this->setParam(param);

					param = new UrlParam();
					param->setValue(THINGSPEAK_PARAM_PRESSURE, env->getPressure());
					this->setParam(param);

					break;
				}
				case AIRQUALITY_MESSAGE: {
					this->handleAirQualityMsg((AirQualityMsg*)msg);
					break;
				}
			}
			return false;
		}

		//@implement
		//@include "ecosense_messages.h"
		void handleAirQualityMsg(AirQualityMsg* msg) {
			UrlParam* param = new UrlParam();
			switch (msg->gasType())
			{
				case AirQualityMsg::GasType::COMMON: {
					param->setValue(THINGSPEAK_PARAM_COMMON, msg->getAmount());
					break;
				}
				case AirQualityMsg::GasType::CO2: {
					param->setValue(THINGSPEAK_PARAM_CO2, msg->getAmount());
					break;
				}
				case AirQualityMsg::GasType::VOCs: {
					param->setValue(THINGSPEAK_PARAM_VOCS, msg->getAmount());
					break;
				}
				case AirQualityMsg::GasType::H2S: {
					param->setValue(THINGSPEAK_PARAM_H2S, msg->getAmount());
					break;
				}
				case AirQualityMsg::GasType::PM25: {
					param->setValue(THINGSPEAK_PARAM_PM25, msg->getAmount());
					break;
				}
				default: {
					TRACELNF("WifiProcess: Unknown GasType at handleAirQualityMsg")
					delete param;
					return;
				}
			}
			this->setParam(param);
		}

		void clearParams() {
			for (int i = this->params.size()-1; i >= 0; i--) {
				delete this->params.remove(i);
			}
		}

		void setParam(UrlParam* param) {
			int pos = this->getParam(param->getId());
			if (pos > -1) {
				delete this->params.remove(pos);
			}
			params.add(param);
		}

		int getParam(uint8_t id) {
			for (byte i = 0; i < this->params.size(); i++) {
				if (this->params.get(i)->getId() == id) {
					return i;
				}
			}
			return -1;
		}
};

#endif
