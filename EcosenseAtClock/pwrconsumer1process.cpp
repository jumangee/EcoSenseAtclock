#include "pwrconsumer1process.h"
#include "pwrconsumer_process.h"
#include "processy_cfg.h"

PwrConsumer1Process::PwrConsumer1Process(uint16_t pId, IProcessMessage* msg) : PwrConsumerProcess(12, taskId, (*(&taskId + 1) - taskId), pId, msg){
    TRACELNF("PwrConsumer1Process::init")
}

static IFirmwareProcess* PwrConsumer1Process::factory(uint16_t pId, IProcessMessage* msg) {
    TRACELNF("PwrConsumer1Process::factory");
    return new PwrConsumer1Process(pId, msg);
}

bool PwrConsumer1Process::handleMessageLogic(IProcessMessage* msg) {
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
