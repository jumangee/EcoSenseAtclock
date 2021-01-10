#define MSG_COMMAND "COMMAND_MESSAGE"
#define MSG_ENV_DATA "ENVDATA_MESSAGE"

#ifndef _METEO_MESSAGES_H
#define _METEO_MESSAGES_H

class IFirmwareProcess;

#include "processy_message.h"
#include <Arduino.h>

class CmdMessage: public IProcessMessage {
	public:
		enum Cmd {
			UNPAUSE_ALL,
			PAUSE_ALL
		};

		CmdMessage(IFirmwareProcess* from, CmdMessage::Cmd cmd): IProcessMessage(from, MSG_COMMAND) {
			this->cmd = cmd;
		}

		CmdMessage(CmdMessage::Cmd cmd): IProcessMessage(NULL, MSG_COMMAND) {
			this->cmd = cmd;
		}

		Cmd getCmd() {
			return this->cmd;
		}

	private:
		Cmd cmd;
};

class EnvDataMessage: public IProcessMessage {
	public:
		EnvDataMessage(IFirmwareProcess* from, float temp, byte humidity, int pressure, float altitude): IProcessMessage(from, MSG_ENV_DATA) {
			this->active = true;
			this->temp =  temp;
			this->humidity = humidity;
			this->pressure = pressure;
			this->altitude = altitude;
		}

		EnvDataMessage(IFirmwareProcess* from): IProcessMessage(NULL, MSG_ENV_DATA) {
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
