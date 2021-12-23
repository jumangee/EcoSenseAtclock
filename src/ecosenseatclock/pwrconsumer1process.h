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
            
            //@implement
            //@include "pwrconsumer_process.h"
            PwrConsumer1Process(IProcessMessage* msg): PwrConsumerProcess(PWRMNGMTPIN_PROCESS1, msg) {    //, taskId, (*(&taskId + 1) - taskId)
                addTask(PRC_MQ136);
                addTask(PRC_CJMCU1100);
                TRACELNF("PwrConsumer1Process::init")
            }

            //@implement
            static IFirmwareProcess* factory(IProcessMessage* msg) {
                return new PwrConsumer1Process(msg);
            }

            //@implement
            bool handleMessageLogic(IProcessMessage* msg) {
                return false;
            }

            uint16_t getNextConsumerId() {
                return PRC_CONSUMER2;
            }
    };

#endif