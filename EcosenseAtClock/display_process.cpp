#include "display_process.h"
#include <SSD1306AsciiWire.h>
#include "stuff.h"

DisplayProcess::DisplayProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	TRACELNF("DisplayProcess::start");
	#if USE_WARNING_LIGHT == 1
	pinMode(WARNLED_R_PIN, OUTPUT);
	pinMode(WARNLED_G_PIN, OUTPUT);
	pinMode(WARNLED_B_PIN, OUTPUT);
	#endif
	
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.clear();
	oled.setFont(MAIN_FONT);
	//oled.print(F("CTAPT..."));	// no chars in font ((
	//temp = 0;
	//clocktick = true;
	/*gasH2S = 0;
	gasCH4 = 0;*/
	temp = 0;
	humidity = 0;
	pressure = 0;
	this->updateScreen = false;
}

static IFirmwareProcess* DisplayProcess::factory(IProcessMessage* msg) {
	return new DisplayProcess(msg);
}

void DisplayProcess::update(unsigned long ms) {
	// oled contrast auto adjustment
	#ifdef PHOTORESISTOR_PIN
		oled.setContrast(100);
	#endif
	if (this->pressure > 0) {
		if (this->temp > 32 || this->temp < 20) {
			this->addWarning(1, SF("Temperature"), this->temp);
		} else {
			this->removeWarning(1);
		}
		if (this->humidity > 50 || this->humidity < 15) {
			this->addWarning(2, SF("Humidity"), this->humidity);
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
	oled.setCursor(0, 3);
	prn2X(this->time);
	if (this->humidity > 0) {
		oled.setCursor(95, 2);
		prn(String(round(this->temp)));
		prn(" c");
		
		oled.setCursor(95, 4);
		prn(String(round(this->humidity)));
		prn(" %");
		oled.setCursor(95, 6);
		prn(String(round(this->pressure)));
		prn("mm");
	}
	//oled.setCursor(0, 0);
	//prn(F("          "));
	//oled.clearField(0, 0, 19);
	for (uint16_t i = 0; i < this->warnings.size(); i++) {
		uint8_t pos = 120-i*1.4;
		if (pos < 1) {
			break;
		}
		oled.setCursor(pos, 0);
		prn(F("!"));
	} 
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
	//clocktick = !clocktick;
	//TRACELN(msg->getTime());
	this->time = msg->getTime();
	this->updateScreen = true;
	/*oled.setFont(MAIN_FONT);
	oled.setCursor(0, 1);
	prn2X(msg->getTime());
	oled.print(msg->getTime());
	if (!msg->getDots()) {
		oled.clearField(15, 0, 1);
	}*/
	//oled.set2X();
	//oled.clearField(0, 0, 5);
	//oled.print(msg->getTime());
	//oled.set1X();
	#if USE_WARNING_LIGHT == 1
		this->updateWarningLight();
	#endif
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
