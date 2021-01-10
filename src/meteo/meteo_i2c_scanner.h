/*
* Simple I2C port scanner process
*/

#ifndef _I2CPORTSCANNER_PROCESS_H
#define _I2CPORTSCANNER_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "meteo_process.h"
#include "meteo_messages.h"

#include <Arduino.h>

class i2cPortScannerProcess: public IFirmwareProcess {
	public:
		//@implement
		//@include <Wire.h>
		i2cPortScannerProcess(String id, IProcessMessage* msg): IFirmwareProcess(id, msg) {
			this->log("i2cPortScannerProcess::start");
		}

		//@implement
		~i2cPortScannerProcess() {
			// stop process
			this->log("i2cPortScannerProcess::stop");
		}

		//@implement
		//@include "stuff.h"
		void update(unsigned long ms) {
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
};

#endif
