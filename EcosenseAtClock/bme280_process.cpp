#include "bme280_process.h"
#include "forcedClimate/forcedClimate.cpp"

BME280SensorProcess::BME280SensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	climateSensor.begin();
}

static IFirmwareProcess* BME280SensorProcess::factory(IProcessMessage* msg) {
	return new BME280SensorProcess(msg);
}

void BME280SensorProcess::update(unsigned long ms) {
	IProcessMessage* msg = readBME280();
	/*if (this->ready) {
		IProcessMessage* msg = readBME280();
		if (msg != NULL) {
			this->getHost()->sendMessage(msg);
		}
	} else {
		this->getHost()->sendMessage(new EnvDataMessage());
	}*/
	this->pause(8000);
}

IProcessMessage* BME280SensorProcess::readBME280() {
	climateSensor.takeForcedMeasurement();
	//EnvDataMessage* msg = new EnvDataMessage(climateSensor.getTemperatureCelcius(), climateSensor.getRelativeHumidity(), climateSensor.getPressure());
	/*TRACEF("TempC=")
	TRACE(climateSensor.getTemperatureCelcius())
	TRACEF(", hum=")
	TRACE(climateSensor.getRelativeHumidity())
	TRACEF(", press=")
	TRACELN(climateSensor.getPressure() / 1.33)*/
	return NULL;
}
