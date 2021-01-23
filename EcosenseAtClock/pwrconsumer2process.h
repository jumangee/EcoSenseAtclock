/**
 * Power Consuming Management Process (GROUP2)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER2_PROCESS_H
#define _PWRCONSUMER2_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    #include "ecosenseatclock.h"
    #include "ecosense_messages.h"
    #include "pwrconsumer_process.h"

    class PwrConsumer2Process: public PwrConsumerProcess {
        private:
            const uint16_t taskId[2] = {PRC_DUMB1, PRC_DUMB2};

        public:
            /**
             * Process consists of 2 task, pwr switch at pin 11
             */
            //PwrConsumer2Process(int pId, IProcessMessage* msg): PwrConsumerProcess(11, PwrConsumer1Process::taskId, 2, pId, msg) {
            PwrConsumer2Process(uint16_t pId, IProcessMessage* msg);

            static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);

            ~PwrConsumer2Process();

    };

#endif