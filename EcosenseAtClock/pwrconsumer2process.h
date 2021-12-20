/**
 * Power Consuming Management Process (GROUP2)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER2_PROCESS_H
#define _PWRCONSUMER2_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    //#include "examplefirmware.h"
    //#include "messages.h"
    #include "pwrconsumer_process.h"
    #include "ecosense_cfg.h"

    
    /**
     * Process consists of 2 task, pwr switch at pin 11
     */
    class PwrConsumer2Process: public PwrConsumerProcess {
        private:
            const uint16_t taskId[2] = {PRC_MQ4, PRC_MQ135};

        public:
            PROCESSID(PRC_CONSUMER2);

            PwrConsumer2Process(IProcessMessage* msg);

            static IFirmwareProcess* factory(IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);
    };

#endif