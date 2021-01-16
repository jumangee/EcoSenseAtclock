#include "meteo_sensors_bme280.h"
#include <Adafruit_BME280.h>
#include "meteo_cfg.h"
#include <Arduino.h>
#include "processy_cfg.h"
#include "stuff.h"

EnvironmentSensorsProcess::EnvironmentSensorsProcess(int pId, IProcessMessage* msg) : IFirmwareProcess(pId, msg){
	//this->log("EnvironmentSensorsProcess::start");
	this->ready = false;
	
	TRACELNF("EnvironmentSensorsProcess::init");
	Wire.begin(); // Wire communication begin
	this->bme = new Adafruit_BME280();
	if (bme->begin(CLOCK2BME280_ADDRESS)) {
		bme->setSampling(Adafruit_BME280::MODE_FORCED,
			Adafruit_BME280::SAMPLING_X1, // temperature
			Adafruit_BME280::SAMPLING_X1, // pressure
			Adafruit_BME280::SAMPLING_X1, // humidity
			Adafruit_BME280::FILTER_OFF);
		
		bme->takeForcedMeasurement();
		TRACELNF("BME: OK");
		this->ready = true;
	} else {
		TRACELNF("BME: ERROR");
	}
}

static IFirmwareProcess* EnvironmentSensorsProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("EnvironmentSensorsProcess::factory");
	return new EnvironmentSensorsProcess(pId, msg);
}

EnvironmentSensorsProcess::~EnvironmentSensorsProcess() {
	// stop process
	TRACELNF("EnvironmentSensorsProcess::stop");
	delete this->bme;
}

void EnvironmentSensorsProcess::update(unsigned long ms) {
	TRACELNF("EnvironmentSensorsProcess::run");
	if (this->ready) {
		IProcessMessage* msg = readBME280();
		if (msg != NULL) {
			this->getHost()->sendMessage(msg);
		}
	} else {
		this->getHost()->sendMessage(new EnvDataMessage(this));
	}
	this->pause(ENVSENSORS_TIMEOUT);
}

IProcessMessage* EnvironmentSensorsProcess::readBME280() {
	Wire.begin(); // Wire communication begin
	bme->takeForcedMeasurement();
	//float alt = bme->readAltitude(SEALEVELPRESSURE_HPA);  // первый замер до усреднения
	float temp = bme->readTemperature();
	//TRACEF("T=");
	//TRACELN(temp);
	byte hum = bme->readHumidity();
	int pres = (float)bme->readPressure() * 0.00750062;
	//alt = ((float)alt * 1 + bme->readAltitude(SEALEVELPRESSURE_HPA)) / 2;  // усреднение, чтобы не было резких скачков (с)НР
	return new EnvDataMessage(NULL, temp, hum, pres);
}
