#include <Wire.h>

#include "ecosenseatclock.h"

void setup() {
	analogReference(EXTERNAL);
	
	Serial.begin(9600);
	delay(1000);
}

void loop() {
	EcosenseAtClockFirmware::get()->run();
}
