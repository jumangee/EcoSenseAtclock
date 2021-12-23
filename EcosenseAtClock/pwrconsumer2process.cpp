#include "pwrconsumer2process.h"
#include "pwrconsumer_process.h"

PwrConsumer2Process::PwrConsumer2Process(IProcessMessage* msg) : PwrConsumerProcess(PWRMNGMTPIN_PROCESS2, msg){
    addTask(PRC_MQ4);
    addTask(PRC_MQ135);
    TRACELNF("PwrConsumer2Process::init")
}

static IFirmwareProcess* PwrConsumer2Process::factory(IProcessMessage* msg) {
    return new PwrConsumer2Process(msg);
}

bool PwrConsumer2Process::handleMessageLogic(IProcessMessage* msg) {
    return false;
}
