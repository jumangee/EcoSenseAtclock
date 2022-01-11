#ifndef _PWRLOADCNTRL_H
#define _PWRLOADCNTRL_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "pinswitch_mngmnt.h"
    
    class PowerloadManagement: public PinSwitchManager {
        public:
            PowerloadManagement(byte *pins, byte keysCnt): PinSwitchManager(pins, keysCnt) {
            }

            static PowerloadManagement* get();
    };
       
#endif