#include "meteo.h"

void setup() {
}

void loop() {
	MeteoClockFirmware::get()->run();
}
