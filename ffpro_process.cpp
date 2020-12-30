#include "ffpro_process.h"
#include "ffpro.h"

void IFirmwareProcess::log(String msg) {
	this->getHost()->log(msg);
}

IFirmware* IFirmwareProcess::getHost() {
	return IFirmware::get();
}