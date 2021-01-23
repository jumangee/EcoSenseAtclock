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
            const uint16_t taskId[2] = {PRC_DUMB1, PRC_DUMB2};

            /**
             * Process consists of 2 task, pwr switch at pin 12
             */
            //PwrConsumer1Process(int pId, IProcessMessage* msg): PwrConsumerProcess(12, PwrConsumer1Process::taskId, 2, pId, msg) { //
            //@implement
            //@include "pwrconsumer_process.h"
            PwrConsumer1Process(uint16_t pId, IProcessMessage* msg): PwrConsumerProcess(12, taskId, (*(&taskId + 1) - taskId), pId, msg) {
                //byte taskId[2] = {PRC_DUMB1, PRC_DUMB2};
                TRACELNF("PwrConsumer1Process::init")
            }

            //@implement
            //@include "processy_cfg.h"
            static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg) {
                TRACELNF("PwrConsumer1Process::factory");
                return new PwrConsumer1Process(pId, msg);
            }

            //@implement
            bool handleMessageLogic(IProcessMessage* msg) {
                switch (msg->getType())
                {
                    case ENVDATA_MESSAGE: { // DUMB1
                        TRACELNF("1handleMessage:ENVDATA_MESSAGE")
                        this->taskDone(PRC_DUMB1);
                        break;
                    }
                    case AIRQUALITY_MESSAGE: {// DUMB2
                        TRACELNF("1handleMessage:AIRQUALITY_MESSAGE")
                        this->taskDone(PRC_DUMB2);
                        break;
                    }
                }
                return false;
            }

            //@implement
            ~PwrConsumer1Process() {
                // stop process
                TRACELNF("PwrConsumer1Process::stop");
            }

    };

#endif