#ifndef _METEOCLOCK_PROCESS_H
#define _METEOCLOCK_PROCESS_H

#include "processy_process.h"

class MeteoClockFirmware;

class IMeteoClockProcess: public IFirmwareProcess {
  public:
    IMeteoClockProcess(int id, IProcessMessage* msg) : IFirmwareProcess(id, msg) {
    }

	//@implement
  	MeteoClockFirmware* getHost() {
		return (MeteoClockFirmware*)IFirmwareProcess::getHost();
	}
};


#endif
