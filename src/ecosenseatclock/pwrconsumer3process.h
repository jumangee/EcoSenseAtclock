/**
 * Power Consuming Management Process (GROUP3)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER3_PROCESS_H
#define _PWRCONSUMER3_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    #include "pwrconsumer_process.h"
    #include "ecosense_cfg.h"
    
    class PwrConsumer3Process: public PwrConsumerProcess {
        public:
            PROCESSID(PRC_CONSUMER3);

            //@implement
            PwrConsumer3Process(IProcessMessage* msg): PwrConsumerProcess(PWRMNGMTPIN_PROCESS3, msg) {
                //addTask(PRC_PPD42NS);
                addTask(PRC_MQ7);
                TRACELNF("PwrConsumer3Process::init")
            }

            //@implement
            static IFirmwareProcess* factory(IProcessMessage* msg) {
                return new PwrConsumer3Process(msg);
            }

            //@implement
            bool handleMessageLogic(IProcessMessage* msg) {
                return false;
            }

            uint16_t getNextConsumerId() {
                return PRC_CONSUMER1;
            }
    };

#endif