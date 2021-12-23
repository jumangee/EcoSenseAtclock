/**
 * Power Consuming Management Process (GROUP1)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER1_PROCESS_H
#define _PWRCONSUMER1_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    #include "ecosense_cfg.h"
    #include "pwrconsumer_process.h"

    class PwrConsumer1Process: public PwrConsumerProcess {
        public:
            PROCESSID(PRC_CONSUMER1);
            
            PwrConsumer1Process(IProcessMessage* msg);

            static IFirmwareProcess* factory(IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);

            uint16_t getNextConsumerId() {
                return PRC_CONSUMER2;
            }
    };

#endif