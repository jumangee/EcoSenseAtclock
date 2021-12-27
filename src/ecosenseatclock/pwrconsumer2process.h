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

            //@implement
            //@include "pwrconsumer_process.h"
            PwrConsumer2Process(IProcessMessage* msg): PwrConsumerProcess(PWRMNGMTPIN_PROCESS2, msg) {
                addTask(PRC_MQ4);
                addTask(PRC_MQ135);
                TRACELNF("PwrConsumer2Process::init")
            }

            //@implement
            static IFirmwareProcess* factory(IProcessMessage* msg) {
                return new PwrConsumer2Process(msg);
            }

            //@implement
            bool handleMessageLogic(IProcessMessage* msg) {
                return false;
            }

            uint16_t getNextConsumerId() {
                return PRC_CONSUMER3;   // skip CONSUMER3 for now
            }
    };

#endif