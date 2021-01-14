#include "meteo_main.h"
#include <SSD1306AsciiWire.h>

MainProcess::MainProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	TRACELNF("MainProcess::start");
	
	Wire.setClock(400000L);
	oled.begin(&Adafruit128x64, OLED_ADDR);
	oled.clear();
	waitAnimPos = 0;
	animTimer = 0;
	
	/*oled.setFont(meteoclockicons);
	oled.setCursor(0, 2);
	oled.print(F("0123456789!@"));
	oled.setCursor(0, 3);
	oled.print(F(" !\"#$%&'()*"));
	oled.setCursor(0, 4);
	oled.print(F("+,-./:;<=>?@"));
	oled.setCursor(0, 5);
	oled.print(F("ABCDEFGHI"));*/
	//oled.setCursor(0, 1);
	//oled.print(F("________________________"));
	//oled.print(F("------------------------"));
	this->freeMem = 101;	// >100 - hide msg
	
	this->updateScreen = false;
}

static IFirmwareProcess* MainProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("MainProcess::factory");
	return new MainProcess(pId, msg);
}

MainProcess::~MainProcess() {
	// stop process
	TRACELNF("MainProcess::stop");
	//delete this->display;
}

void MainProcess::update(unsigned long ms) {
	/*this->animTimer += ms;
	if (this->animTimer > ANIM_WAIT_TIME) {
		this->animTimer = 0;
		this->waitAnimPos++;
		if (this->waitAnimPos > 7) {
			this->waitAnimPos = 0;
		}
	}
	oled.setCursor(113, 5);
	oled.setFont(meteoclockicons);
	oled.print(String(String(F("ABCDEFGH")).charAt(waitAnimPos)));*/
	if (this->updateScreen) {
		this->render();
		this->updateScreen = false;
	}
	this->pause(3);
}

void MainProcess::render() {
	oled.setFont(System5x7);
	oled.clearField(0, 0, 4);
	{
		bool warn = false;
		if (this->temp > 30 || this->temp < 18) {
			oled.setCursor(5, 0);
			oled.print(F("!"));
			warn = true;
		}
		if (this->humidity > 35 || this->humidity < 20) {
			oled.setCursor(10, 0);
			oled.print(F("!"));
			warn = true;
		}
		if (warn) {
			oled.setCursor(0, 0);
			oled.print(F("["));
			oled.setCursor(15, 0);
			oled.print(F("]"));
		} else {
			oled.setCursor(0, 0);
			oled.print(F("OK"));
		}
	}
	oled.clearField(0, 2, 5);
	oled.setCursor(0, 2);
	oled.print(F("T: "));
	oled.print(this->temp);
	oled.print(F(" C"));
	oled.clearField(0, 3, 5);			
	oled.setCursor(0, 3);
	oled.print(F("H: "));
	oled.print(this->humidity);
	oled.print(F(" %"));
	oled.clearField(0, 4, 5);
	oled.setCursor(0, 4);			
	oled.print(F("P: "));
	oled.print(this->pressure);
	oled.print(F(" mm"));
	if (this->freeMem < 61) {
		oled.clearField(8, 4, 8);
		oled.setCursor(85, 1);
		oled.print(F("Mem "));
		oled.print( this->freeMem );
		oled.print(F("%"));
	}
}

bool MainProcess::handleMessage(IProcessMessage* msg) {
	switch (msg->getType())
	{
		case ENVDATA_MESSAGE: {
			this->handleEnvDataMsg((EnvDataMessage*)msg);
			return true;
		}
		case FREEMEM_MESSAGE: {
			this->handleMemUsageMsg((MemUsageMessage*)msg);
			return true;
		}
		case AIRQUALITY_MESSAGE: {
			this->handleAirQualityMsg((AirQualityMsg*)msg);
			return true;
		}
		case CURTIME_MESSAGE: {
			this->handleTimeMsg((CurrentTimeMsg*)msg);
			return true;
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

void MainProcess::handleMemUsageMsg(MemUsageMessage* msg) {
	this->freeMem = msg->getFreemem();
}

void MainProcess::handleTimeMsg(CurrentTimeMsg* msg) {
	oled.setFont(Stang5x7);
	String time = msg->getTime();
	oled.setCursor(0, 0);
	oled.set2X();
	//oled.clearField(0, 0, 5);
	oled.print(time);
	oled.set1X();
}
