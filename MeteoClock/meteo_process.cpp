#include "meteo_process.h"

MeteoClockFirmware* IMeteoClockProcess::getHost() {
		return (MeteoClockFirmware*)IFirmwareProcess::getHost();
	}
