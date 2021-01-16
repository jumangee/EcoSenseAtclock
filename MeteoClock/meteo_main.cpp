#include "meteo_main.h"
#include <SSD1306AsciiWire.h>

MainProcess::MainProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	TRACELNF("MainProcess::start");
	
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.clear();
	temp = 0;
	clocktick = true;
	gasH2S = 0;
	gasCH4 = 0;
	printGasInfo(SPRITE_GAS_H2S, 2, this->gasH2S);
	printGasInfo(SPRITE_GAS_CH4, 5, this->gasCH4);
	this->updateScreen = false;
}

static IFirmwareProcess* MainProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MainProcess::factory");
	return new MainProcess(pId, msg);
}

MainProcess::~MainProcess() {
	// stop process
	TRACELNF("MainProcess::stop");
}

void MainProcess::update(unsigned long ms) {
	if (this->updateScreen) {
		this->render();
		this->updateScreen = false;
	}
	if (this->gasH2S > 0) {
		printGasInfo(SPRITE_GAS_H2S, 2, this->gasH2S);
	}
	if (this->gasCH4 > 0) {
		printGasInfo(SPRITE_GAS_CH4, 5, this->gasCH4);
	}
	this->pause(5);
}

void MainProcess::render() {
	oled.setFont(ICONS_FONT);
	oled.setCursor(0, 2);
	oled.print(F("( "));
	oled.setFont(MAIN_FONT);
	oled.set2X();
	oled.print(this->temp);
	oled.setFont(ICONS_FONT);
	oled.set1X();
	oled.print(F(")"));
	oled.setCursor(0, 4);
	oled.print(F("# "));
	if (this->humidity > 0) {
		oled.setFont(MAIN_FONT);
		oled.set2X();
		oled.print(this->humidity);
		oled.set1X();
		oled.setRow(oled.row()+1);
		oled.print(F("%"));
	} else {
		oled.setFont(MAIN_FONT);
		oled.set2X();
		oled.print(F("-   "));
		oled.set1X();
	}
	oled.setCursor(0, 6);			
	oled.setFont(ICONS_FONT);
	oled.print(F("- "));
	if (this->pressure > 0) {
		oled.setFont(MAIN_FONT);
		oled.set2X();
		oled.print(this->pressure);
		oled.setFont(ICONS_FONT);
		oled.set1X();
	} else {
		oled.setFont(MAIN_FONT);
		oled.set2X();
		oled.print(F("-   "));
		oled.set1X();
	}
}

bool MainProcess::handleMessage(IProcessMessage* msg) {
	switch (msg->getType())
	{
		case ENVDATA_MESSAGE: {
			this->handleEnvDataMsg((EnvDataMessage*)msg);
			break;
		}
		case AIRQUALITY_MESSAGE: {
			this->handleAirQualityMsg((AirQualityMsg*)msg);
			break;
		}
		case CURTIME_MESSAGE: {
			this->handleTimeMsg((CurrentTimeMsg*)msg);
			return true; // dispose
		}
		case WIFISTATE_MESSAGE: {
			this->handleWifiMsg((WiFiStateMsg*)msg);
			return true; // dispose
		}
		
	}
	return false;
}

void MainProcess::handleEnvDataMsg(EnvDataMessage* msg) {
	if (msg->isActive()) {
		/*String s = SF("BME data: T=");
		s += msg->getTemp();
		s += F("C, Hum=");
		s += msg->getHumidity();
		s += F("%, Pres=");
		s += msg->getPressure();
		TRACELN(s);*/
		if (this->temp != msg->getTemp()) {
			this->temp = msg->getTemp();
			this->updateScreen = true;
		}
		if (this->humidity != msg->getHumidity()) {
			this->humidity = msg->getHumidity();
			this->updateScreen = true;
		}
		if (this->pressure != msg->getPressure()) {
			this->pressure = msg->getPressure();
			this->updateScreen = true;
		}
	} else {
		TRACELNF("BME Sensors is passive!");
	}
}

void MainProcess::handleTimeMsg(CurrentTimeMsg* msg) {
	oled.setFont(Stang5x7);
	oled.setCursor(0, 0);
	oled.set2X();
	//oled.clearField(0, 0, 5);
	oled.print(msg->getTime());
	oled.set1X();
	clocktick = !clocktick;
}
