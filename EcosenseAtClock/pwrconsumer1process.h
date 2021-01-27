/**
 * Power Consuming Management Process (GROUP1)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER1_PROCESS_H
#define _PWRCONSUMER1_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    #include "ecosenseatclock.h"
    #include "ecosense_messages.h"
    #include "pwrconsumer_process.h"

    class PwrConsumer1Process: public PwrConsumerProcess {
        private:
        public:
            const uint16_t taskId[2] = {PRC_MQ7SENSOR, PRC_MHZ19SENSOR};

            /**
             * Process consists of 2 task, pwr switch at pin 12
             */
            //PwrConsumer1Process(int pId, IProcessMessage* msg): PwrConsumerProcess(12, PwrConsumer1Process::taskId, 2, pId, msg) { //
            PwrConsumer1Process(uint16_t pId, IProcessMessage* msg);

            static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);
    };

#endif