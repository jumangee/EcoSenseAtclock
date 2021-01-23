#include "pwrconsumer2process.h"
#include "pwrconsumer_process.h"
#include "processy_cfg.h"

PwrConsumer2Process::PwrConsumer2Process(uint16_t pId, IProcessMessage* msg) : PwrConsumerProcess(11, taskId, (*(&taskId + 1) - taskId), pId, msg){
    //byte taskId[2] = {PRC_DUMB1, PRC_DUMB2};
    TRACELNF("PwrConsumer2Process::init");
}

static IFirmwareProcess* PwrConsumer2Process::factory(uint16_t pId, IProcessMessage* msg) {
    TRACELNF("PwrConsumer2Process::factory");
    return new PwrConsumer2Process(pId, msg);
}

bool PwrConsumer2Process::handleMessageLogic(IProcessMessage* msg) {
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

PwrConsumer2Process::~PwrConsumer2Process() {
    // stop process
    TRACELNF("PwrConsumer2Process::stop");
}
