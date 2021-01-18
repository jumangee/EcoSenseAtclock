#include "meteo_wifi_wifiesp_lite.h"
#include "meteo_cfg.h"
#include "EspDrv/RingBuffer.cpp"
#include "EspDrv/EspDrv.cpp"
#include <Arduino.h>
#include "processy_cfg.h"
#include "EspDrv/EspDrv.h"

WifiProcess::WifiProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
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

static IFirmwareProcess* WifiProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("WifiProcess::factory");
	return new WifiProcess(pId, msg);
}

WifiProcess::~WifiProcess() {
	// stop process
	TRACELNF("WifiProcess::stop");
	if (espSerial != NULL) {
		delete espSerial;
	}
}

void WifiProcess::update(unsigned long ms) {
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

bool WifiProcess::WiFiConnect() {
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

void WifiProcess::WiFiDisconnect() {
	EspDrv::disconnect();
	this->getHost()->sendMessage(new WiFiStateMsg(false));
}

void WifiProcess::simpleSendData() {
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

bool WifiProcess::handleMessage(IProcessMessage* msg) {
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
