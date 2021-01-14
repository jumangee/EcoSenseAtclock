#include "meteo_wifi.h"
#include <Adafruit_BME280.h>
#include "meteo_cfg.h"
#include <Arduino.h>
#include "processy_cfg.h"
#include "stuff.h"

WifiProcess::WifiProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	//this->log("WifiProcess::start");
	TRACELNF("WifiProcess::init");
	//this->sendCommand(F("AT+RST"), F("Ready"));
	this->ready = false;
	this->pause(5000);
}

static IFirmwareProcess* WifiProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("WifiProcess::factory");
	return new WifiProcess(pId, msg);
}

WifiProcess::~WifiProcess() {
	// stop process
	TRACELNF("WifiProcess::stop");
}

void WifiProcess::update(unsigned long ms) {
	/*TRACELNF("WifiProcess::run");
	if (!this->ready) {
		this->sendCommand("AT+CWMODE=1","OK");
		this->sendCommand("AT+CIFSR", "OK");
		this->ready = true;
	}*/
	
	this->pause(ENVSENSORS_TIMEOUT);
}
