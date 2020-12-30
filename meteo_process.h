/*
* MeteClock Firmware: process parent
*/
 
#ifndef _METEOCLOCK_PROCESS_H
#define _METEOCLOCK_PROCESS_H

class MeteoClockFirmware;

class IMeteoClockProcess: public IFirmwareProcess {
  public:
    IMeteoClockProcess(String id, IProcessMessage* msg) : IFirmwareProcess(id, msg) {
    }
    
  	/*MeteoClockFirmware* getHost() {
  		return (MeteoClockFirmware*)getHost();
  	}*/
};


#endif
