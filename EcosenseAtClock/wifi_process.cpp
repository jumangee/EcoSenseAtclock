#include "wifi_process.h"
#include "ecosense_cfg.h"
#include "EspDrv/RingBuffer.cpp"
#include "EspDrv/EspDrv.cpp"
#include <Arduino.h>
#include "swserialsingleton.h"
#include "EspDrv/EspDrv.h"
#include "ecosense_messages.h"

WifiProcess::WifiProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	// initializers count must be equal to THINGSPEAK_CHANNELS!
	dataSendTask[THINGSPEAKCHANNEL_COMMON] = new ThingspeakWebSendTask(THINGSPEAK_CHANNEL1_KEY);
	dataSendTask[THINGSPEAKCHANNEL_AIRQUALITY] = new ThingspeakWebSendTask(THINGSPEAK_CHANNEL2_KEY);
	this->state = NONE;
	this->pause(30000);
}

static IFirmwareProcess* WifiProcess::factory(IProcessMessage* msg) {
	return new WifiProcess(msg);
}

WifiProcess::~WifiProcess() {
	// stop process
	TRACELNF("WifiProcess::stop");
	espStop();
	for (byte i = 0; i < THINGSPEAK_CHANNELS; i++) {
		delete dataSendTask[i];
	}
}

void WifiProcess::espStop() {
	if (espSerial != NULL) {
		if (EspDrv::getConnectionStatus() == WL_CONNECTED) {			
			EspDrv::disconnect();
		}
		espSerial->end();
		espSerial = SoftwareSerialSingleton::unlock();
	}
	state = ReportState::NONE;
}

void WifiProcess::update(unsigned long ms) {
	//unsigned long now = millis();
	switch (state) {
		case ReportState::NONE: {
			if ( dataReadyToSend() ) {
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
			if (WiFiConnect()) {
				state = ReportState::CONNECTED;
				this->pause(100);
			} else {
				//this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::ERROR));
				this->pause(15000);
				espStop();
			}
			return;
		}
		case ReportState::CONNECTED: {
			for (byte i = 0; i < THINGSPEAK_CHANNELS; i++) {
				if ( dataSendTask[i]->size > 0 ) {
					simpleSendData(dataSendTask[i]);
				}
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

bool WifiProcess::WiFiConnect() {
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

void WifiProcess::simpleSendData(ThingspeakWebSendTask *task) {
	//uint8_t _sock = 1;
	String server = THINGSPEAK_SERVER;
	if (EspDrv::startClient(server.c_str(), 80, 1, TCP_MODE)) {
		TRACEF("Connected to server ");
		TRACELN(server);
		{
			String buf = SF("GET ");
			buf += THINGSPEAKREPORT_URI;
			buf += task->getApiKey();
			sendPacket(buf);
			for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
				UrlParam p = task->getParam(i);
				if (p.active) {
					sendPacket(SF("&field"));
					sendPacket(String(i + 1));
					sendPacket(SF("="));
					sendPacket(p.getValue());
				}
			}
			buf = F(" HTTP/1.1\r\nHost: ");
			buf += server;
			buf += F("\r\nConnection: close\r\n\r\n");
			sendPacket(buf);
		}
		EspDrv::stopClient(1);
		task->clear();
	}
}

bool WifiProcess::handleMessage(IProcessMessage* msg) {
	switch (msg->getType())
	{
		case ENVDATA_MESSAGE: {
			EnvDataMessage* env = (EnvDataMessage*)msg;
			ThingspeakWebSendTask* task = dataSendTask[THINGSPEAKCHANNEL_COMMON];
			
			task->getParam(THINGSPEAK_PARAM_TEMP).setValue(env->temp);
			task->getParam(THINGSPEAK_PARAM_HUMIDITY).setValue(env->humidity);
			task->getParam(THINGSPEAK_PARAM_PRESSURE).setValue(env->pressure);
			task->recount();
			return false;
		}
		case SELFREPORT_MESSAGE: {
			SelfReportMessage* report = (SelfReportMessage*)msg;
			ThingspeakWebSendTask* task = dataSendTask[THINGSPEAKCHANNEL_COMMON];
			
			task->getParam(THINGSPEAK_PARAM_PROCESSES).setValue((uint16_t)report->processCount);
			task->getParam(THINGSPEAK_PARAM_FREEMEM).setValue(report->freemem);
			task->recount();
			return false;
		}
		case AIRQUALITY_MESSAGE: {
			AirQualityMsg* gas = (AirQualityMsg*)msg;
			ThingspeakWebSendTask* task = dataSendTask[THINGSPEAKCHANNEL_AIRQUALITY];
			switch (gas->gasType())
			{
				case AirQualityMsg::GasType::COMMON: {
					task->getParam(THINGSPEAK_PARAM_COMMON).setValue(gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::CO2: {
					task->getParam(THINGSPEAK_PARAM_CO2).setValue((uint16_t)gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::H2S: {
					task->getParam(THINGSPEAK_PARAM_H2S).setValue(gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::VOCs: {
					task->getParam(THINGSPEAK_PARAM_VOCS).setValue(gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::CO: {
					task->getParam(THINGSPEAK_PARAM_CO).setValue(gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::PM25: {
					task->getParam(THINGSPEAK_PARAM_PM25).setValue(gas->getAmount());
					break;
				}
			}
			task->recount();
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
