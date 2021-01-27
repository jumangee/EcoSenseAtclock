#include "pwrconsumer3process.h"
#include "pwrconsumer_process.h"
#include "processy_cfg.h"

PwrConsumer3Process::PwrConsumer3Process(uint16_t pId, IProcessMessage* msg) : PwrConsumerProcess(10, taskId, (*(&taskId + 1) - taskId), pId, msg){
    //byte taskId[2] = {PRC_DUMB1, PRC_DUMB2};
    TRACELNF("PwrConsumer3Process::init");
}

static IFirmwareProcess* PwrConsumer3Process::factory(uint16_t pId, IProcessMessage* msg) {
    TRACELNF("PwrConsumer3Process::factory");
    return new PwrConsumer3Process(pId, msg);
}

bool PwrConsumer3Process::handleMessageLogic(IProcessMessage* msg) {
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
