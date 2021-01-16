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
	TRACELN("Initializing ESP module")
	EspDrv::wifiDriverInit(espSerial); 
	// check for the presence of the shield
	if (EspDrv::getConnectionStatus() == WL_NO_SHIELD) {
		TRACELNF("WiFi shield not present");
		delete this->espSerial;
		this->espSerial = NULL;
	} else {
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
			{
				//String url = dataSendTask.getUrl(F(THINGSPEAK_CHANNEL_KEY));
				//String server = dataSendTask.getServer();
				simpleSendData(/*server, url*/);
			}
			dataSendTask.clear();
			WiFiDisconnect();
			TRACELN("[WIFI] Send packets done")
			lastReportTime = millis();
		} else {
			TRACELNF("[WIFI] Failed to clearing tasks buf!")
			dataSendTask.clear();
		}
	}
	this->pause(ENVSENSORS_TIMEOUT);
}

bool WifiProcess::handleMessage(IProcessMessage* msg) {
	if (!this->ready) return;
	switch (msg->getType())
	{
		case ENVDATA_MESSAGE: {
			EnvDataMessage* env = (EnvDataMessage*)msg;
			if (env->isActive()) {
				this->dataSendTask.setParam(1, env->getTempF());
			}
			return false;
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
