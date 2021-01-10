#include "meteo_sensors.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "meteo_cfg.h"
#include <Arduino.h>
#include "processy_cfg.h"
#include "stuff.h"

EnvironmentSensorsProcess::EnvironmentSensorsProcess(String id, IProcessMessage* msg) : IFirmwareProcess(id, msg){
	//this->log("EnvironmentSensorsProcess::start");
	this->ready = false;
	this->initDone = false;
	this->bme = new Adafruit_BME280();
	/*Wire.begin(); // Wire communication begin
	if (bme.begin(CLOCK2BME280_ADDRESS)) {
		bme.setSampling(Adafruit_BME280::MODE_FORCED,
			Adafruit_BME280::SAMPLING_X1, // temperature
			Adafruit_BME280::SAMPLING_X1, // pressure
			Adafruit_BME280::SAMPLING_X1, // humidity
			Adafruit_BME280::FILTER_OFF);
		
		bme.takeForcedMeasurement();
		this->log("BME: OK");
		this->ready = true;
	} else {
		this->log("BME: ERROR");
	}*/
}

void EnvironmentSensorsProcess::init() {
	this->log("EnvironmentSensorsProcess::init");
	Wire.begin(); // Wire communication begin
	if (bme->begin(CLOCK2BME280_ADDRESS)) {
		bme->setSampling(Adafruit_BME280::MODE_FORCED,
			Adafruit_BME280::SAMPLING_X1, // temperature
			Adafruit_BME280::SAMPLING_X1, // pressure
			Adafruit_BME280::SAMPLING_X1, // humidity
			Adafruit_BME280::FILTER_OFF);
		
		bme->takeForcedMeasurement();
		this->log("BME: OK");
		this->ready = true;
	} else {
		this->log("BME: ERROR");
	}
	this->initDone = true;
}

void EnvironmentSensorsProcess::update(unsigned long ms) {
	this->log("EnvironmentSensorsProcess::run");
	if (!this->initDone) {
		this->init();
		return;	// work - next run
	}
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
	this->log("GET BME DATA...");
	Wire.begin(); // Wire communication begin
	bme->takeForcedMeasurement();
	float alt = bme->readAltitude(SEALEVELPRESSURE_HPA);  // первый замер до усреднения
	float temp = bme->readTemperature();
	byte hum = bme->readHumidity();
	int pres = (float)bme->readPressure() * 0.00750062;
	alt = ((float)alt * 1 + bme->readAltitude(SEALEVELPRESSURE_HPA)) / 2;  // усреднение, чтобы не было резких скачков (с)НР
	return new EnvDataMessage(this, temp, hum, pres, alt);
}
