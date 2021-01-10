#include "meteo_i2c_scanner.h"
#include <Wire.h>
#include "stuff.h"

i2cPortScannerProcess::i2cPortScannerProcess(String id, IProcessMessage* msg) : IFirmwareProcess(id, msg){
	this->log("i2cPortScannerProcess::start");
}

i2cPortScannerProcess::~i2cPortScannerProcess() {
	// stop process
	this->log("i2cPortScannerProcess::stop");
}

void i2cPortScannerProcess::update(unsigned long ms) {
	this->log("i2cPortScannerProcess::run");
	byte error, address; //variable for error and I2C address
	int nDevices;
	this->log("Scanning...");
	Wire.begin(); // Wire communication begin
	nDevices = 0;
	for (address = 1; address < 127; address++ )
	{
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.print(address, HEX);
			Serial.println("  !");
			nDevices++;
		}
		else if (error == 4) {
			Serial.print("Unknown error at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");
	this->pause(5000);
}
