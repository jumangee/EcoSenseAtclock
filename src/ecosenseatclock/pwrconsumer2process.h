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
            //@implement
            //@include "pwrconsumer_process.h"
            PwrConsumer2Process(uint16_t pId, IProcessMessage* msg): PwrConsumerProcess(11, taskId, (*(&taskId + 1) - taskId), pId, msg) {
                //byte taskId[2] = {PRC_DUMB1, PRC_DUMB2};
                TRACELNF("PwrConsumer2Process::init");
            }

            //@implement
            static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg) {
                TRACELNF("PwrConsumer2Process::factory");
                return new PwrConsumer2Process(pId, msg);
            }

            //@implement
            //@include "processy_cfg.h"
            bool handleMessageLogic(IProcessMessage* msg) {
                switch (msg->getType())
                {
                    case ENVDATA_MESSAGE: { // DUMB1
                        TRACELNF("2handleMessage:ENVDATA_MESSAGE")
                        this->taskDone(PRC_DUMB1);
                        break;
                    }
                    case AIRQUALITY_MESSAGE: {// DUMB2
                        TRACELNF("2handleMessage:AIRQUALITY_MESSAGE")
                        this->taskDone(PRC_DUMB2);
                        break;
                    }
                }
                return false;
            }
    };

#endif