#include <Wire.h>

#include "meteo.h"

void setup() {
	analogReference(EXTERNAL);
	
	Serial.begin(9600);
	delay(1000);
}

void loop() {
	MeteoClockFirmware::get()->run();
}
