#include "pwrload_mngmnt.h"

//PowerloadManagement* PowerloadManagement::instance = NULL;
#include "ecosenseatclock.h"

static PowerloadManagement* PowerloadManagement::get() {
    EcosenseAtClockFirmware* host = EcosenseAtClockFirmware::get();
    return host->getPwrLoadMngmnt();
}
