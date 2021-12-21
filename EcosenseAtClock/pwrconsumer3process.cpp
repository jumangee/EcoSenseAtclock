#include "pwrconsumer3process.h"
#include "pwrconsumer_process.h"
#include "processy_cfg.h"

PwrConsumer3Process::PwrConsumer3Process(IProcessMessage* msg) : PwrConsumerProcess(10, msg){    //taskId, (*(&taskId + 1) - taskId)
    addTask(PRC_MHZ19);
    //addTask(PRC_MQ7);
}

static IFirmwareProcess* PwrConsumer3Process::factory(IProcessMessage* msg) {
    //TRACELNF("PwrConsumer3Process::factory");
    return new PwrConsumer3Process(msg);
}

bool PwrConsumer3Process::handleMessageLogic(IProcessMessage* msg) {
    return false;
}
