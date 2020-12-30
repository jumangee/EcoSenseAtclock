#include "meteo.h"
#include "meteo_main.h"
#include "meteo_sensors.h"

void setup() {
}

void loop() {
	MeteoClockFirmware::get()->run();
}
