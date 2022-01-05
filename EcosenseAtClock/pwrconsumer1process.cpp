#include "pwrconsumer1process.h"
#include "pwrconsumer_process.h"

PwrConsumer1Process::PwrConsumer1Process(IProcessMessage* msg) : PwrConsumerProcess(PWRMNGMTPIN_PROCESS1, msg){
    addTask(PRC_MQ136);
    addTask(PRC_CJMCU1100);
    TRACELNF("PwrConsumer1Process::init")
}

static IFirmwareProcess* PwrConsumer1Process::factory(IProcessMessage* msg) {
    return new PwrConsumer1Process(msg);
}

bool PwrConsumer1Process::handleMessageLogic(IProcessMessage* msg) {
    return false;
}
