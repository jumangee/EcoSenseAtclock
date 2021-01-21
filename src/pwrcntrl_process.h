/**
 * Power Management Process
 * for Processy Firmware Framework
 */

#ifndef _PWRMNGMNT_PROCESS_H
#define _PWRMNGMNT_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "pwrloadctrl.h"

class PwrMngmntProcess: public IFirmwareProcess {
	private:
        byte        keyPin;
        uint32_t    poweredTime;

	public:
		//@implement
		//@include "processy_cfg.h"
		PwrMngmntProcess(byte keyPin, int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			TRACELNF("PwrMngmntProcess::init");
			this->keyPin = keyPin;
		}

        //@implement
		//@include "processy_cfg.h"
		//@include "pwrloadctrl.h"
		unsigned long run(unsigned long start) {
            this->poweredTime = PowerloadManagement::get()->requestLoadSwitch(this->pin);
			if (this->poweredTime > 0) {
                IFirmwareProcess::run(start);
            }
		}

        uint32_t getPoweredTime() {
            return this->poweredTime;
        }

        //@implement
        void releaseLoad() {
            PowerloadManagement::get()->releaseLoad(this->pin);
        }

		//@implement
		//@include <Arduino.h>
		~PwrMngmntProcess() {
			// stop process
            this->releaseLoad();
			TRACELNF("PwrMngmntProcess::stop");
		}

};

#endif