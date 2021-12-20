//#include <Wire.h>

#include "ecosenseatclock.h"

void setup() {
}

void loop() {
	EcosenseAtClockFirmware::get()->run();
}
