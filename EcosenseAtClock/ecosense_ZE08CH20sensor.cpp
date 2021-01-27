#include "ecosense_ZE08CH20sensor.h"

static IFirmwareProcess* ZE08CH2OSensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("ZE08CH2OSensorProcess::factory");
	return new ZE08CH2OSensorProcess(pId, msg);
}
