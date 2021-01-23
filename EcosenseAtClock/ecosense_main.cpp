#include "ecosense_main.h"
#include <SSD1306AsciiWire.h>
#include "stuff.h"

MainProcess::MainProcess(uint16_t pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	TRACELNF("MainProcess::start");
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
	temp = 0;
	clocktick = true;
	gasH2S = 0;
	gasCH4 = 0;
	temp = 0;
	humidity = 0;
	pressure = 0;
	this->updateScreen = false;
}

static IFirmwareProcess* MainProcess::factory(uint16_t pId, IProcessMessage* msg) {
	TRACELNF("MainProcess::factory");
	return new MainProcess(pId, msg);
}

MainProcess::~MainProcess() {
	// stop process
	TRACELNF("MainProcess::stop");
}

void MainProcess::update(unsigned long ms) {
	// oled contrast auto adjustment
	#ifdef PHOTORESISTOR_PIN
		oled.setContrast(100);
	#endif
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
	this->pause(10);
}

void MainProcess::render() {
	if (prnEnvData(SPRITE_ENV_TEMP, 2, this->temp)) {
		oled.setFont(ICONS_FONT);
		prn(SPRITE_ENV_C);
		prn(SPRITE_SPC);
	}
	if (prnEnvData(SPRITE_ENV_HUM, 4, this->humidity)) {
		oled.setFont(ICONS_FONT);
		prn(SPRITE_ENV_C);prn(SPRITE_ENV_C);
		oled.print(0x25);	//%
		prn(SPRITE_SPC);
	}
	if (prnEnvData(SPRITE_ENV_PRES, 6, this->pressure)) {
		prn(SPRITE_SPC);
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
	}
}

void MainProcess::handleTimeMsg(CurrentTimeMsg* msg) {
	clocktick = !clocktick;
	if (!msg->getHrs() && !msg->getMins()) {
		return;
	}
	oled.setFont(MAIN_FONT);
	oled.setCursor(0, 0);
	//prn2X(msg->getTime());
	oled.print(msg->getTime());
	if (!msg->getDots()) {
		oled.clearField(15, 0, 1);
	}
	//oled.set2X();
	//oled.clearField(0, 0, 5);
	//oled.print(msg->getTime());
	//oled.set1X();
	#if USE_WARNING_LIGHT == 1
		this->updateWarningLight();
	#endif
}

void MainProcess::printGasInfo(char g, byte row, byte quality) {
	if (quality > 0) {
		oled.setFont(MAIN_FONT);
		if (quality > 1) {
			oled.setInvertMode(clocktick);
		} else {
			oled.setInvertMode(false);
		}
		oled.setCursor(90, row);
		oled.setFont(ICONS_FONT);
		prn(g);
		prn(SPRITE_SPC);
	
		oled.setCol(108);
		if (quality == 1) {
			prn(SPRITE_OK);
		} else if (quality == 2) {
			prn(SPRITE_WARNING);
		} else {
			prn(SPRITE_DANGER);
		}
		prn(SPRITE_SPC);
		oled.setInvertMode(false);
	}
}

void MainProcess::handleAirQualityMsg(AirQualityMsg* msg) {
	switch (msg->gasType())
	{
		case H2S: {
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
		}
		/*case CO2: {
			if (this->gasCO2 != msg->getQuality()) {
				this->gasCO2 = msg->getQuality();
				printGasInfo(SPRITE_GAS_CO2, SCREENROW_GAS_CO2, this->gasCO2);
			}
			return;
		}*/
	}
}

void MainProcess::handleWifiMsg(WiFiStateMsg* msg) {
	oled.setFont(ICONS_FONT);
	oled.setCursor(100, 0);
	oled.print(F("   "));
	oled.setCursor(108, 0);
	if (msg->isActive()) {
		prn(SPRITE_WIFI);
		prn(SPRITE_SPC);
	}
}
