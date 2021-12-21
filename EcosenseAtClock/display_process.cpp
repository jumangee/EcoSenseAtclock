#include "display_process.h"
#include <SSD1306AsciiWire.h>

DisplayProcess::DisplayProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	TRACELNF("DisplayProcess::start");
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.clear();
	oled.setFont(MAIN_FONT);
	timeDots = true;
	temp = 0;
	humidity = 0;
	pressure = 0;
	this->updateScreen = false;
}

static IFirmwareProcess* DisplayProcess::factory(IProcessMessage* msg) {
	return new DisplayProcess(msg);
}

void DisplayProcess::update(unsigned long ms) {
	if (this->pressure > 0) {
		if (this->temp > 32 || this->temp < 20) {
			this->addWarning(1, this->temp);
		} else {
			this->removeWarning(1);
		}
		if (this->humidity > 50 || this->humidity < 15) {
			this->addWarning(2, this->humidity);
		} else {
			this->removeWarning(2);
		}
	}
	if (this->updateScreen) {
		this->render();
		this->updateScreen = false;
	}
	/*if (this->gasH2S > 0) {
		printGasInfo(SPRITE_GAS_H2S, SCREENROW_GAS_H2S, this->gasH2S);
	}
	if (this->gasCH4 > 0) {
		printGasInfo(SPRITE_GAS_CH4, SCREENROW_GAS_CH4, this->gasCH4);
	}*/
	this->pause(42);
}

void DisplayProcess::render() {
	// time
	oled.setCursor(0, 3);
	oled.set2X();
	if (this->timeH < 10) {
		oled.print(F("0"));
	}
	oled.print(this->timeH);
	oled.print(this->timeDots ? F(":") : F(" "));
	if (this->timeM < 10) {
		oled.print(F("0"));
	}
	oled.print(this->timeM);
	oled.set1X();
	// info
	if (this->humidity > 0) {
		oled.setCursor(95, 2);
		oled.print(round(this->temp));
		oled.print(F(" c"));
		
		oled.setCursor(95, 4);
		oled.print(round(this->humidity));
		oled.print(F(" %"));
		oled.setCursor(95, 6);
		oled.print(round(this->pressure));
		oled.print(F("mm"));
	}
	//oled.setCursor(0, 0);
	//prn(F("          "));
	// warnings
	oled.setCursor(0, 0);
	oled.set2X();
	oled.clearToEOL();
	//oled.setInvertMode(true);
	for (uint16_t i = 0; i < this->warnings.size(); i++) {
		uint8_t pos = 118-i*10;
		if (pos < 1) {
			break;
		}
		oled.setCursor(pos, 0);
		oled.print(F("!"));
	}
	oled.set1X();
	//oled.setInvertMode(false);
}

bool DisplayProcess::handleMessage(IProcessMessage* msg) {
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
	}
	return false;
}

void DisplayProcess::handleEnvDataMsg(EnvDataMessage* msg) {
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
}

void DisplayProcess::handleTimeMsg(CurrentTimeMsg* msg) {
	//TRACELN(msg->getTime());
	this->timeH	= msg->getHrs();
	this->timeM	= msg->getMins();
	this->timeDots = !this->timeDots;
	this->updateScreen = true;
}

void DisplayProcess::handleAirQualityMsg(AirQualityMsg* msg) {
	switch (msg->gasType())
	{
		/*case H2S: {
			if (this->gasH2S != msg->getQuality()) {
				this->gasH2S = msg->getQuality();
				printGasInfo(SPRITE_GAS_H2S, SCREENROW_GAS_H2S, this->gasH2S);
			}
			return;
		}
		case CH4: {
			if (this->gasCH4 != msg->getQuality()) {
				this->gasCH4 = msg->getQuality();
				printGasInfo(SPRITE_GAS_CH4, SCREENROW_GAS_CH4, this->gasCH4);
			}
			return;
		}*/
		/*case CO2: {
			if (this->gasCO2 != msg->getQuality()) {
				this->gasCO2 = msg->getQuality();
				printGasInfo(SPRITE_GAS_CO2, SCREENROW_GAS_CO2, this->gasCO2);
			}
			return;
		}*/
	}
}
