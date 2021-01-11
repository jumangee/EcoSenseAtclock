#include "meteo_main.h"
#include <SSD1306AsciiWire.h>
#include "stuff.h"

MainProcess::MainProcess(String id, IProcessMessage* msg) : IFirmwareProcess(id, msg){
	this->log("MainProcess::start");
	
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.setFont(System5x7);
	this->getHost()->addProcess(PRC_SENSORS);
}

MainProcess::~MainProcess() {
	// stop process
	this->log("MainProcess::stop");
	//delete this->display;
}

void MainProcess::update(unsigned long ms) {
	//this->log("MainProcess::run");
	oled.clear();
	oled.print("Hello world!");
	this->pause(5000);
}

bool MainProcess::handleMessage(IProcessMessage* msg) {
	/*if (msg->getType() == MSG_ENV_DATA) {
		EnvDataMessage* msg = (EnvDataMessage*)msg;
		if (msg->isActive()) {
			this->log(S("BME data: T=", String(msg->getTemp()).c_str(),
			"C, Hum=", String(msg->getHumidity()).c_str(), "%, Pres=", 
			String(msg->getPressure()).c_str(), "mm, Alt=",
			String(msg->getAltitude()).c_str(), "m"));
		} else {
			this->log("BME Sensors is passive!");
		}
	}*/
	return false;
}
