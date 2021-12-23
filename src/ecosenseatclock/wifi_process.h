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

class WifiProcess: public IFirmwareProcess {
	private:
		SoftwareSerial 			*espSerial;
		//unsigned long			lastReportTime = 0;
		ThingspeakWebSendTask	dataSendTask;
		enum ReportState {
			NONE = 0,
			READY,
			CONNECTED,
			SENT
		}						state;
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

			state = ReportState::NONE;

			espSerial = new SoftwareSerial(WIFI_RX_PIN, WIFI_TX_PIN); // RX, TX
			espSerial->begin(19200);

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
			if (espSerial != NULL) {
				delete espSerial;
			}
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "ecosense_cfg.h"
		//@include "EspDrv/EspDrv.h"
		//@include "ecosense_messages.h"
		void update(unsigned long ms) {
			unsigned long now = millis();

			/*if (this->dataSendTask.size > 0 && (now - lastReportTime > REPORT_TIMEOUT)) {
				if (WiFiConnect()) {
					simpleSendData();
					dataSendTask.clear();
					WiFiDisconnect();
					TRACELN("[WIFI] Send packets done")

					lastReportTime = millis();
				} else {
					TRACELNF("[WIFI] Failed to clearing tasks buf!")
				}
			}*/

			switch (state) {
				case ReportState::NONE: {
					TRACELN("Initializing ESP module...")
					EspDrv::wifiDriverInit(espSerial);

					if (EspDrv::getConnectionStatus() == WL_NO_SHIELD) {
						TRACELNF("[WiFi] FAIL");
						/*delete this->espSerial;
						this->espSerial = NULL;
						this->stop();*/

						state = ReportState::NONE;
						this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::NONE));
					} else {
						TRACELNF("[WiFi] OK");
						state = ReportState::READY;
					}
					break;
				}
				case ReportState::READY: {
					if ( this->dataSendTask.size > 0 ) {
						if (WiFiConnect()) {
							state = ReportState::CONNECTED;
						} else {
							//this->sendMessage(new WifiStateMessage(F("CONN ERR")));
							this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::ERROR));
							this->pause(15000);
							return;
						}
					}
					break;
				}
				case ReportState::CONNECTED: {
					//this->sendMessage(new WifiStateMessage(F("SENDING")));
					simpleSendData();
					state = ReportState::SENT;
					//this->sendMessage(new WifiStateMessage(F("SENT")));
					TRACELN("[WIFI] Send packets done")
					break;
				}
				case ReportState::SENT: {
					WiFiDisconnect();
					state = ReportState::NONE;
					//this->sendMessage(new WifiStateMessage(F("READY")));
					//lastReportTime = millis();
					this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::OK));
					
					this->pause(REPORT_TIMEOUT);
					return;
				}
			}

			this->pause(2000);
		}

		//@implement
		bool WiFiConnect() {
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
			//this->getHost()->sendMessage(new WiFiStateMsg(false));
			return false;
		}

		//@implement
		void WiFiDisconnect() {
			EspDrv::disconnect();
			//this->getHost()->sendMessage(new WiFiStateMsg(false));
		}

		//@implement
		void simpleSendData() {
			uint8_t _sock = 1;
			String server = THINGSPEAK_SERVER;
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

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					EnvDataMessage* env = (EnvDataMessage*)msg;
					
					dataSendTask.getParam(THINGSPEAK_PARAM_TEMP).setValue(env->getTemp());
					dataSendTask.getParam(THINGSPEAK_PARAM_HUMIDITY).setValue(env->getHumidity());
					dataSendTask.getParam(THINGSPEAK_PARAM_PRESSURE).setValue(env->getPressure());

					dataSendTask.recount();
					return false;
				}
				case AIRQUALITY_MESSAGE: {
					//this->handleAirQualityMsg((AirQualityMsg*)msg);
					AirQualityMsg* gas = (AirQualityMsg*)msg;
					switch (gas->gasType())
					{
						case AirQualityMsg::GasType::COMMON: {
							dataSendTask.getParam(THINGSPEAK_PARAM_COMMON).setValue(gas->getAmount());
							break;
						}
						case AirQualityMsg::GasType::CO2: {
							dataSendTask.getParam(THINGSPEAK_PARAM_CO2).setValue(gas->getAmount());
							break;
						}
						case AirQualityMsg::GasType::H2S: {
							dataSendTask.getParam(THINGSPEAK_PARAM_H2S).setValue(gas->getAmount());
							break;
						}
						case AirQualityMsg::GasType::VOCs: {
							dataSendTask.getParam(THINGSPEAK_PARAM_VOCS).setValue(gas->getAmount());
							break;
						}
					}

					dataSendTask.recount();
					return false;
				}
			}
		}
};

#endif
