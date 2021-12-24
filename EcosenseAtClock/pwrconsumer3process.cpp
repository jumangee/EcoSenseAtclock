#include "pwrconsumer3process.h"

PwrConsumer3Process::PwrConsumer3Process(IProcessMessage* msg) : PwrConsumerProcess(PWRMNGMTPIN_PROCESS3, msg){
    //addTask(PRC_PPD42NS);
    //addTask(PRC_MQ7);
    TRACELNF("PwrConsumer3Process::init")
}

static IFirmwareProcess* PwrConsumer3Process::factory(IProcessMessage* msg) {
    return new PwrConsumer3Process(msg);
}

bool PwrConsumer3Process::handleMessageLogic(IProcessMessage* msg) {
    return false;
}
