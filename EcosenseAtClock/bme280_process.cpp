#include "bme280_process.h"
#include "ecosense_messages.h"
#include "forcedClimate/forcedClimate.cpp"

BME280SensorProcess::BME280SensorProcess(IProcessMessage* msg) : IFirmwareProcess(msg){
	climateSensor.begin();
}

static IFirmwareProcess* BME280SensorProcess::factory(IProcessMessage* msg) {
	return new BME280SensorProcess(msg);
}

void BME280SensorProcess::update(unsigned long ms) {
	climateSensor.takeForcedMeasurement();
	this->sendMessage(new EnvDataMessage(climateSensor.getTemperatureCelcius(), climateSensor.getRelativeHumidity(), climateSensor.getPressure() / 1.33));
	this->pause(8000);
}
