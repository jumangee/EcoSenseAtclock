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
            //@implement
            //@include "pwrconsumer_process.h"
            PwrConsumer3Process(uint16_t pId, IProcessMessage* msg): PwrConsumerProcess(10, taskId, (*(&taskId + 1) - taskId), pId, msg) { 
                //byte taskId[2] = {PRC_DUMB1, PRC_DUMB2};
                TRACELNF("PwrConsumer3Process::init");
            }

            //@implement
            //@include "processy_cfg.h"
            static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg) {
                TRACELNF("PwrConsumer3Process::factory");
                return new PwrConsumer3Process(pId, msg);
            }

            //@implement
            bool handleMessageLogic(IProcessMessage* msg) {
                switch (msg->getType())
                {
                    case ENVDATA_MESSAGE: { // DUMB1
                        TRACELNF("3handleMessage:ENVDATA_MESSAGE")
                        this->taskDone(PRC_DUMB1);
                        break;
                    }
                    case AIRQUALITY_MESSAGE: {// DUMB2
                        TRACELNF("3handleMessage:AIRQUALITY_MESSAGE")
                        this->taskDone(PRC_DUMB2);
                        break;
                    }
                }
                return false;
            }

    };

#endif