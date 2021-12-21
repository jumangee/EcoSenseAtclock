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
            //const uint16_t taskId[2] = {PRC_MQ4, PRC_MQ135};

        public:
            PROCESSID(PRC_CONSUMER2);

            //@implement
            //@include "pwrconsumer_process.h"
            PwrConsumer2Process(IProcessMessage* msg): PwrConsumerProcess(PWRMNGMTPIN_PROCESS2, msg) {  //taskId, (*(&taskId + 1) - taskId)
                addTask(PRC_MQ4);
                addTask(PRC_MQ135);
                TRACELNF("PwrConsumer2Process::init");
            }

            //@implement
            static IFirmwareProcess* factory(IProcessMessage* msg) {
                //TRACELNF("PwrConsumer2Process::factory");
                return new PwrConsumer2Process(msg);
            }

            //@implement
            //@include "processy_cfg.h"
            bool handleMessageLogic(IProcessMessage* msg) {
                return false;
            }
    };

#endif