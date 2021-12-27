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
		SoftwareSerial 			*espSerial = NULL;
		
		ThingspeakWebSendTask*	dataSendTask1;
		ThingspeakWebSendTask*	dataSendTask2;

		enum ReportState {
			NONE = 0,
			READY,
			CONNECTED,
			SENT,
			DISABLED
		}						state;
	public:
		PROCESSID(PRC_WIFI);

		//@implement
		//@include "ecosense_cfg.h"
		//@include "EspDrv/RingBuffer.cpp"
		//@include "EspDrv/EspDrv.cpp"
		WifiProcess(IProcessMessage* msg): IFirmwareProcess(msg) {
			dataSendTask1 = new ThingspeakWebSendTask(THINGSPEAK_CHANNEL1_KEY);
			dataSendTask2 = new ThingspeakWebSendTask(THINGSPEAK_CHANNEL2_KEY);

			TRACELNF("WifiProcess::init");
			this->state = NONE;
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
			espStop();
		}

		//@implement
		//@include "swserialsingleton.h"
		void espStop() {
			if (espSerial != NULL) {
				if (EspDrv::getConnectionStatus() == WL_CONNECTED) {			
					EspDrv::disconnect();
				}

				espSerial->end();
				espSerial = SoftwareSerialSingleton::unlock();
			}
			state = ReportState::NONE;
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "ecosense_cfg.h"
		//@include "EspDrv/EspDrv.h"
		//@include "ecosense_messages.h"
		//@include "swserialsingleton.h"
		void update(unsigned long ms) {
			//unsigned long now = millis();

			switch (state) {
				case ReportState::NONE: {
					if ( this->dataSendTask1->size > 0 || this->dataSendTask2->size > 0) {
						espSerial = SoftwareSerialSingleton::get(WIFI_RX_PIN, WIFI_TX_PIN, 19200);
						if (espSerial != NULL) {
							TRACELN("Initializing ESP module...")
							EspDrv::wifiDriverInit(espSerial);

							if (EspDrv::getConnectionStatus() == WL_NO_SHIELD) {
								TRACELNF("[WiFi] FAIL");
								espStop();
								this->pause();
								return;
							}
							TRACELNF("[WiFi] OK");
							state = ReportState::READY;
						}
					}
					this->pause(500);
					return;
				}
				case ReportState::READY: {
					if ( this->dataSendTask1->size > 0 || this->dataSendTask2->size > 0) {
						if (WiFiConnect()) {
							state = ReportState::CONNECTED;
							this->pause(100);
						} else {
							//this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::ERROR));
							this->pause(15000);
							espStop();
						}
					}
					return;
				}
				case ReportState::CONNECTED: {
					if ( dataSendTask1->size > 0) {
						simpleSendData(dataSendTask1);
					}
					if ( dataSendTask2->size > 0) {
						simpleSendData(dataSendTask2);
					}
					state = ReportState::SENT;

					TRACELN("[WIFI] Send packets done")
					break;
				}
				case ReportState::SENT: {
					espStop();
					//this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::OK));
					
					this->pause(REPORT_TIMEOUT);
					return;
				}
			}
		}

		//@implement
		bool WiFiConnect() {
			// attempt to connect to WiFi network
			if (EspDrv::getConnectionStatus() != WL_CONNECTED) {
				TRACELNF("[WIFI] Attempting to connect");
				if (EspDrv::wifiConnect(SF(WIFI_SSID).c_str(), SF(WIFI_PWD).c_str())) {
					//TRACELNF("[WIFI] Connected to the network");
					//this->dataSendTask.setParam(7, uint16_t(abs(EspDrv::getCurrentRSSI())));	//byte((double(EspDrv::getCurrentRSSI()) / 65535.0*100.0))
					return true;
				}/* else {
					TRACELNF("[WIFI] Connection error");
				}*/
			} else {
				return true;
			}
			return false;
		}

		//@implement
		void simpleSendData(ThingspeakWebSendTask *task) {
			uint8_t _sock = 1;
			String server = THINGSPEAK_SERVER;
			if (EspDrv::startClient(server.c_str(), 80, _sock, TCP_MODE)) {
				TRACEF("Connected to server ");
				TRACELN(server);
				{
					String buf = SF("GET ");
					buf += THINGSPEAKREPORT_URI;
					buf += task->getApiKey();
					buf += task->getUrl(); 
					buf += F(" HTTP/1.1\r\nHost: ");
					buf += server;
					buf += F("\r\nConnection: close\r\n\r\n");
					TRACELN(buf);
					if (!EspDrv::sendData(_sock, buf.c_str(), buf.length())) {
						TRACELNF("[simpleSendData] Error");
					}
				}
				EspDrv::stopClient(_sock);
				task->clear();
			}
		}

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					EnvDataMessage* env = (EnvDataMessage*)msg;
					
					dataSendTask1->getParam(THINGSPEAK_PARAM_TEMP).setValue(env->getTemp());
					dataSendTask1->getParam(THINGSPEAK_PARAM_HUMIDITY).setValue(env->getHumidity());
					dataSendTask1->getParam(THINGSPEAK_PARAM_PRESSURE).setValue(env->getPressure());

					dataSendTask1->recount();
					return false;
				}
				case AIRQUALITY_MESSAGE: {
					AirQualityMsg* gas = (AirQualityMsg*)msg;
					switch (gas->gasType())
					{
						case AirQualityMsg::GasType::COMMON: {
							dataSendTask2->getParam(THINGSPEAK_PARAM_COMMON).setValue(gas->getAmount());
							break;
						}
						case AirQualityMsg::GasType::CO2: {
							dataSendTask2->getParam(THINGSPEAK_PARAM_CO2).setValue(gas->getAmount());
							break;
						}
						case AirQualityMsg::GasType::H2S: {
							dataSendTask2->getParam(THINGSPEAK_PARAM_H2S).setValue(gas->getAmount());
							break;
						}
						case AirQualityMsg::GasType::VOCs: {
							dataSendTask2->getParam(THINGSPEAK_PARAM_VOCS).setValue(gas->getAmount());
							break;
						}
					}

					dataSendTask2->recount();
					return false;
				}
				/*case BTNCLICK_MESSAGE: {
					if (((ButtonClickMessage*)msg)->event == ButtonClickMessage::ButtonEvent::HOLD) {
						if (this->state == DISABLED) {
							this->state = NONE;
						} else {
							espStop();
							this->state = DISABLED;
						}
					}
				}*/
			}
			return false;
		}
};

#endif
