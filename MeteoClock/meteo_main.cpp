#include "meteo_main.h"
#include <SSD1306AsciiWire.h>
#include "stuff.h"

MainProcess::MainProcess(String & id, IProcessMessage* msg) : IFirmwareProcess(id, msg){
	TRACELNF("MainProcess::start");
	
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.setFont(System5x7);
	oled.clear();
	
	this->updateScreen = false;
}

MainProcess::~MainProcess() {
	// stop process
	TRACELNF("MainProcess::stop");
	//delete this->display;
}

void MainProcess::update(unsigned long ms) {
	//this->log("MainProcess::run");
	if (this->updateScreen) {
		this->render();
		this->updateScreen = false;
	}
}

bool MainProcess::handleMessage(IProcessMessage* msg) {
	if (msg->getType() == ENVDATA_MESSAGE) {
		this->handleEnvDataMsg((EnvDataMessage*)msg);
		return true;
	}
	return false;
}
