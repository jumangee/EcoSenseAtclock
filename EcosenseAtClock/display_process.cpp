#include "display_process.h"
#include <SSD1306AsciiWire.h>
#include "ecosense_messages.h"

DisplayProcess::DisplayProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.clear();
	oled.setFont(MAIN_FONT);
}

static IFirmwareProcess* DisplayProcess::factory(IProcessMessage* msg) {
	return new DisplayProcess(msg);
}

void DisplayProcess::update(unsigned long ms) {
	if (this->pressure > 0) {
		int warnPos = this->findWarning(1);
		if (this->temp > 32 || this->temp < 20) {
			if (warnPos == -1)
				this->addWarning(1, this->temp);
		} else if (warnPos > -1) {
			this->removeWarning(1);
		}
		warnPos = this->findWarning(2);
		if (this->humidity > 50 || this->humidity < 15) {
			if (warnPos == -1)
				this->addWarning(2, this->humidity);
		} else if (warnPos > -1) {
			this->removeWarning(2);
		}
	}
	if (showWarningNum > this->warnings.size()-1) {
		showWarningNum = -1;
		oled.clear();
		updateScreen = true;
		updateWarnings = true;
	}
	if (this->updateScreen) {
		this->render();
	}
	if (this->updateWarnings) {
		this->renderWarnings();
	}
	this->pause(42);
}

void DisplayProcess::renderMainScreen() {
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
	/*if (this->co2 > 0) {
		oled.setCursor(95, 7);
		oled.print((uint16_t)co2);
		oled.print(F("co2"));
		oled.print(F(" "));
	}*/
}

void DisplayProcess::renderWarningScreen() {
	oled.set2X();
	oled.setCursor(0, 2);
	WarningInfo* warn = this->warnings.get(this->showWarningNum);
	switch (warn->id) {
		case 1: oled.print(F("TEMPERATURE")); break;
		case 2: oled.print(F("HUMIDITY")); break;
		case 10: oled.print(F("AIR QUALITY")); break;
		case 11: oled.print(F("H2S")); break;
		case 12: oled.print(F("CO")); break;
		//case 13: oled.print(F("SO2")); break;
		case 14: oled.print(F("CO2")); break;
		//case 15: oled.print(F("CH4")); break;
		//case 16: oled.print(F("CH2O")); break;
		//case 17: oled.print(F("C6H5_CH3")); break;
		//case 18: oled.print(F("PM1")); break;
		case 19: oled.print(F("PM25")); break;
		case 20: oled.print(F("VOCs")); break;
		/*default: {
			oled.print(warn->id);
		}*/
	}
	oled.setCursor(0, 5);
	oled.print(warn->value);
	oled.set1X();
}

void DisplayProcess::render() {
	if (showWarningNum == -1) {
		renderMainScreen();
	}
	else {	// WARNINGS MODE
		renderWarningScreen();
	}			
	this->updateScreen = false;
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
		case BTNCLICK_MESSAGE: {
			if (warnings.size() > 0) {
				if (this->showWarningNum == -1) {
					oled.clear();
				}
				this->showWarningNum++;
				this->updateWarnings = true;
				this->updateScreen = true;
			}
			return true; // dispose
		}
	}
	return false;
}

void DisplayProcess::handleEnvDataMsg(EnvDataMessage* msg) {
	//if (this->temp != msg->getTemp()) {
		this->temp = msg->getTemp();
	//	this->updateScreen = true;
	//}
	//if (this->humidity != msg->getHumidity()) {
		this->humidity = msg->getHumidity();
	//	this->updateScreen = true;
	//}
	//if (this->pressure != msg->getPressure()) {
		this->pressure = msg->getPressure();
		this->updateScreen = true;
	//}
}

void DisplayProcess::handleTimeMsg(CurrentTimeMsg* msg) {
	this->timeH	= msg->getHrs();
	this->timeM	= msg->getMins();
	this->timeDots = !this->timeDots;
	this->updateScreen = true;
}

void DisplayProcess::handleAirQualityMsg(AirQualityMsg* msg) {
	uint16_t gasCode = static_cast<uint16_t>(msg->gasType()) + 10;
	uint16_t gasLevel = static_cast<uint16_t>(msg->getConcentration());
	if (gasLevel > 1) {
		this->addWarning(gasCode, msg->getAmount());
	} else {
		this->removeWarning(gasCode);
	}
	/*if (msg->gasType() == AirQualityMsg::GasType::CO2) {
		this->co2 = msg->getAmount();
	}*/
}