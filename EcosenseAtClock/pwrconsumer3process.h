/**
 * Power Consuming Management Process (GROUP3)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER3_PROCESS_H
#define _PWRCONSUMER3_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    #include "ecosenseatclock.h"
    #include "ecosense_messages.h"
    #include "pwrconsumer_process.h"

    class PwrConsumer3Process: public PwrConsumerProcess {
        private:
            const uint16_t taskId[2] = {PRC_DUMB1, PRC_DUMB2};

        public:
            /**
             * Process consists of 2 task, pwr switch at pin 10
             */
            //PwrConsumer3Process(int pId, IProcessMessage* msg): PwrConsumerProcess(10, PwrConsumer1Process::taskId, 2, pId, msg) {   //PwrConsumer3Process::taskId
            PwrConsumer3Process(uint16_t pId, IProcessMessage* msg);

            static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);

    };

#endif