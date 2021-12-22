#include "wifi_process.h"
#include "ecosense_cfg.h"
#include "EspDrv/RingBuffer.cpp"
#include "EspDrv/EspDrv.cpp"
#include <Arduino.h>
#include "processy_cfg.h"
#include "EspDrv/EspDrv.h"

WifiProcess::WifiProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	//this->log("WifiProcess::start");
	TRACELNF("WifiProcess::init");
	pinMode(WIFI_RX_PIN, INPUT);
  			pinMode(WIFI_TX_PIN, OUTPUT);
	lastReportTime = 0;
	espSerial = new SoftwareSerial(WIFI_RX_PIN, WIFI_TX_PIN); // RX, TX
	espSerial->begin(19200);
	TRACELN("Initializing ESP module...")
	EspDrv::wifiDriverInit(espSerial); 
	// check for the presence of the shield
	if (EspDrv::getConnectionStatus() == WL_NO_SHIELD) {
		TRACELNF("[WiFi] FAIL");
		delete this->espSerial;
		this->espSerial = NULL;
		this->stop();
		return;
	}
	TRACELNF("[WiFi] OK");
	this->pause(30000);
}

static IFirmwareProcess* WifiProcess::factory(IProcessMessage* msg) {
	return new WifiProcess(msg);
}

WifiProcess::~WifiProcess() {
	// stop process
	TRACELNF("WifiProcess::stop");
	if (espSerial != NULL) {
		delete espSerial;
	}
}

void WifiProcess::update(unsigned long ms) {
	unsigned long now = millis();
	if (this->dataSendTask.size > 0 && (now - lastReportTime > REPORT_TIMEOUT)) {
		if (WiFiConnect()) {
			simpleSendData();
			dataSendTask.clear();
			WiFiDisconnect();
			TRACELN("[WIFI] Send packets done")
			lastReportTime = millis();
		} else {
			TRACELNF("[WIFI] Failed to clearing tasks buf!")
		}
	}
	this->pause(15000);
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
		} else {
			TRACELNF("[WIFI] Connection error");
		}
	} else {
		return true;
	}
	//this->getHost()->sendMessage(new WiFiStateMsg(false));
	return false;
}

void WifiProcess::WiFiDisconnect() {
	EspDrv::disconnect();
	//this->getHost()->sendMessage(new WiFiStateMsg(false));
}

void WifiProcess::simpleSendData() {
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

bool WifiProcess::handleMessage(IProcessMessage* msg) {
	switch (msg->getType())
	{
		case ENVDATA_MESSAGE: {
			EnvDataMessage* env = (EnvDataMessage*)msg;
			
			dataSendTask.getParam(THINGSPEAK_PARAM_TEMP).setValue(THINGSPEAK_PARAM_TEMP, env->getTemp());
			dataSendTask.getParam(THINGSPEAK_PARAM_HUMIDITY).setValue(THINGSPEAK_PARAM_HUMIDITY, env->getHumidity());
			dataSendTask.getParam(THINGSPEAK_PARAM_PRESSURE).setValue(THINGSPEAK_PARAM_PRESSURE, env->getPressure());
			break;
		}
		case AIRQUALITY_MESSAGE: {
			//this->handleAirQualityMsg((AirQualityMsg*)msg);
			AirQualityMsg* gas = (AirQualityMsg*)msg;
			switch (gas->gasType())
			{
				case AirQualityMsg::GasType::COMMON: {
					dataSendTask.getParam(THINGSPEAK_PARAM_COMMON).setValue(THINGSPEAK_PARAM_COMMON, gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::CO2: {
					dataSendTask.getParam(THINGSPEAK_PARAM_CO2).setValue(THINGSPEAK_PARAM_CO2, gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::H2S: {
					dataSendTask.getParam(THINGSPEAK_PARAM_H2S).setValue(THINGSPEAK_PARAM_H2S, gas->getAmount());
					break;
				}
				case AirQualityMsg::GasType::VOCs: {
					dataSendTask.getParam(THINGSPEAK_PARAM_VOCS).setValue(THINGSPEAK_PARAM_VOCS, gas->getAmount());
					break;
				}
			}
			break;
		}
	}
	dataSendTask.recount();
	return false;
}