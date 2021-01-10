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
		i2cPortScannerProcess(String id, IProcessMessage* msg);

		~i2cPortScannerProcess();

		void update(unsigned long ms);
};

#endif
