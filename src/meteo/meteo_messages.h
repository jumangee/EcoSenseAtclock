#ifndef _METEO_MESSAGES_H
#define _METEO_MESSAGES_H

class IFirmwareProcess;

#include "processy_message.h"
#include <Arduino.h>
#include <math.h>

//--- MSG TYPES -------------------
#define ENVDATA_MESSAGE		1001
#define CURTIME_MESSAGE		1002
#define AIRQUALITY_MESSAGE	1003
#define WIFISTATE_MESSAGE	1004
//---------------------------------

class EnvDataMessage: public IProcessMessage {
	public:
		EnvDataMessage(float t, byte h, int p): IProcessMessage(NULL, ENVDATA_MESSAGE) {
			this->active = true;
			this->temp =  t;
			this->humidity = h;
			this->pressure = p;
		}

		EnvDataMessage(): IProcessMessage(NULL, ENVDATA_MESSAGE) {
			this->active = false;
		}

		bool isActive() {
			return this->active;
		}

		int getTemp() {
			return int(round(this->temp));
		}

		float getTempF() {
			return this->temp;
		}

		byte getHumidity() {
			return this->humidity;
		}

		int	getPressure() {
			return this->pressure;
		}

		void setCO2(int co2) {
			this->co2 = co2;
		}

		int getCO2() {
			return co2;
		}

	private:
		bool	active;
		float	temp;
		byte	humidity;
		int		pressure;
		int		co2;
};


class CurrentTimeMsg: public IProcessMessage {
	public:
		CurrentTimeMsg(int8_t hrs, int8_t mins, boolean dotFlag): IProcessMessage(NULL, CURTIME_MESSAGE) {
			this->hrs =  hrs;
			this->mins = mins;
			this->dotFlag = dotFlag;
		}

		int8_t getHrs() {
			return this->hrs;
		}

		int8_t getMins() {
			return this->mins;
		}

		boolean	getDots() {
			return this->dotFlag;
		}

		String getTime() {
			String time;
			time.reserve(5);
			if (getHrs() < 10) {
				time += String(F("0"));
			}
			time += String(getHrs());
			time += getDots() ? F(":") : F(" ");
			if (getMins() < 10) {
				time += String(F("0"));
			}
			time += getMins();
			return time;
		}

	private:
		int8_t hrs, mins, secs;
        boolean dotFlag;
};


enum AirQualityGasType {
	H2S,
	CO,
	SO2,
	CO2,
	CH4
};

class AirQualityMsg: public IProcessMessage {
	public:
		/**
		 * gasType: measured type
		 * quality: 0-42 - norm, 43-84 - high, 84-127 - danger
		 */
		AirQualityMsg(AirQualityGasType gasType, byte quality, float v): IProcessMessage(NULL, AIRQUALITY_MESSAGE) {
			this->gas =  gasType;
			this->quality = quality;
			this->voltage = v;
		}

		AirQualityGasType gasType() {
			return this->gas;
		}

		byte getQuality() {
			return (float(this->quality) / 42) + 1;
		}

		float getVoltage() {
			return this->voltage;
		}

	private:
		AirQualityGasType gas;
		byte	quality;
		float	voltage;
};

class WiFiStateMsg: public IProcessMessage {
	public:
		WiFiStateMsg(bool active): IProcessMessage(NULL, WIFISTATE_MESSAGE) {
			this->active =  active;
		}

		bool isActive() {
			return this->active;
		}

	private:
		bool active;
};

#endif
