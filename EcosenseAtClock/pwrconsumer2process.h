/**
 * Power Consuming Management Process (GROUP2)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER2_PROCESS_H
#define _PWRCONSUMER2_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    #include "pwrconsumer_process.h"
    #include "ecosense_cfg.h"

    class PwrConsumer2Process: public PwrConsumerProcess {
        public:
            PROCESSID(PRC_CONSUMER2);

            PwrConsumer2Process(IProcessMessage* msg);

            static IFirmwareProcess* factory(IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);

            uint16_t getNextConsumerId() {
                return PRC_CONSUMER3;
            }
    };

#endif