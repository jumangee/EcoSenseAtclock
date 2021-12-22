/**
 * Power Consuming Management Process (GROUP3)
 * for Processy Firmware Framework
 */

#ifndef _PWRCONSUMER3_PROCESS_H
#define _PWRCONSUMER3_PROCESS_H

    #include "processy.h"
    #include "processy_process.h"
    #include "processy_cfg.h"

    //#include "examplefirmware.h"
    //#include "messages.h"
    #include "pwrconsumer_process.h"
    #include "ecosense_cfg.h"
    
    /**
     * Process consists of 2 task, pwr switch at pin 10
     */
    class PwrConsumer3Process: public PwrConsumerProcess {
        private:
            //const uint16_t taskId[1] = {PRC_MHZ19}; //, PRC_MQ7

        public:
            PROCESSID(PRC_CONSUMER3);

            PwrConsumer3Process(IProcessMessage* msg);

            static IFirmwareProcess* factory(IProcessMessage* msg);

            bool handleMessageLogic(IProcessMessage* msg);

            uint16_t getNextConsumerId() {
                return PRC_CONSUMER1;
            }
    };

#endif