#ifndef _METEO_MESSAGES_H
#define _METEO_MESSAGES_H

class IFirmwareProcess;

#include "processy_message.h"
#include <Arduino.h>

//--- MSG TYPES -------------------
#define ENVDATA_MESSAGE 1001
//---------------------------------

class EnvDataMessage: public IProcessMessage {
	public:
		EnvDataMessage(IFirmwareProcess* from, float t, byte h, int p, float a): IProcessMessage(from, ENVDATA_MESSAGE) {
			this->active = true;
			this->temp =  t;
			this->humidity = h;
			this->pressure = p;
			this->altitude = a;
		}

		EnvDataMessage(IFirmwareProcess* from): IProcessMessage(from, ENVDATA_MESSAGE) {
			this->active = false;
		}

		bool isActive() {
			return this->active;
		}

		float getTemp() {
			return this->temp;
		}

		byte getHumidity() {
			return this->humidity;
		}

		int	getPressure() {
			return this->pressure;
		}

		float getAltitude() {
			return this->altitude;
		}

	private:
		bool	active;
		float	temp;
		byte	humidity;
		int		pressure;
		float	altitude;  //int
};

#endif
