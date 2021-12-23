#include "wifi_process.h"
#include "ecosense_cfg.h"
#include "EspDrv/RingBuffer.cpp"
#include "EspDrv/EspDrv.cpp"
#include <Arduino.h>
#include "processy_cfg.h"
#include "EspDrv/EspDrv.h"
#include "ecosense_messages.h"

WifiProcess::WifiProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	//this->log("WifiProcess::start");
	TRACELNF("WifiProcess::init");
	espSerial.begin(19200);
	TRACELN("Initializing ESP module...")
	EspDrv::wifiDriverInit(&espSerial);
	if (EspDrv::getConnectionStatus() == WL_NO_SHIELD) {
		TRACELNF("[WiFi] FAIL");
		/*delete this->espSerial;
		this->espSerial = NULL;
		this->stop();*/
		//state = ReportState::NONE;
		//this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::NONE));
		this->pause();
		return;
	}
	TRACELNF("[WiFi] OK");
	state = ReportState::READY;
	this->pause(30000);
}

static IFirmwareProcess* WifiProcess::factory(IProcessMessage* msg) {
	return new WifiProcess(msg);
}

WifiProcess::~WifiProcess() {
	// stop process
	TRACELNF("WifiProcess::stop");
	//if (espSerial != NULL) {
		//delete espSerial;
	//}
}

void WifiProcess::update(unsigned long ms) {
	//unsigned long now = millis();
	/*if (this->dataSendTask.size > 0) {
		if (WiFiConnect()) {
			simpleSendData();
			dataSendTask.clear();
			EspDrv::disconnect();
			TRACELN("[WIFI] Send packets done")
			this->pause(REPORT_TIMEOUT);
			return;
		} else {
			TRACELNF("[WIFI] Failed to clearing tasks buf!")
		}
	}*/
	switch (state) {
		case ReportState::READY: {
			if ( this->dataSendTask.size > 0 ) {
				if (WiFiConnect()) {
					state = ReportState::CONNECTED;
					this->pause(100);
				} else {
					//this->sendMessage(new WifiStateMessage(F("CONN ERR")));
					this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::ERROR));
					this->pause(15000);
				}
			}
			return;
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
			EspDrv::disconnect();
			state = ReportState::READY;
			//this->sendMessage(new WifiStateMessage(F("READY")));
			//lastReportTime = millis();
			this->sendMessage(new WifiEventMessage(WifiEventMessage::WifiEvent::OK));
			
			this->pause(REPORT_TIMEOUT);
			return;
		}
	}
	this->pause(1000);
}

bool WifiProcess::WiFiConnect() {
	// attempt to connect to WiFi network
	//this->getHost()->sendMessage(new WiFiStateMsg(true));
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
	//this->getHost()->sendMessage(new WiFiStateMsg(false));
	return false;
}

void WifiProcess::simpleSendData() {
	uint8_t _sock = 1;
	String server = THINGSPEAK_SERVER;
	if (EspDrv::startClient(server.c_str(), 80, _sock, TCP_MODE)) {
		TRACEF("Connected to server ");
		TRACELN(server);
		{
			String buf = SF("GET ");
			buf += THINGSPEAKREPORT_URI;
			buf += THINGSPEAK_CHANNEL_KEY;
			buf += dataSendTask.getUrl(); 
			buf += F(" HTTP/1.1\r\nHost: ");
			buf += server;
			buf += F("\r\nConnection: close\r\n\r\n");
			TRACELN(buf);
			if (!EspDrv::sendData(_sock, buf.c_str(), buf.length())) {
				TRACELNF("[simpleSendData] Error");
			}
		}
		EspDrv::stopClient(_sock);
		dataSendTask.clear();
	}
}

bool WifiProcess::handleMessage(IProcessMessage* msg) {
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
					//dataSendTask.getParam(THINGSPEAK_PARAM_CO2).setValue(gas->getAmount());
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
	return false;
}
