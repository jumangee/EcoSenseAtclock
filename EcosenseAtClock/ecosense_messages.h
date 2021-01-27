#ifndef _ECOSENSEATCLOCK_MESSAGES_H
#define _ECOSENSEATCLOCK_MESSAGES_H

class IFirmwareProcess;

#include "processy_message.h"
#include <Arduino.h>
#include <math.h>

//--- MSG TYPES -------------------
#define ENVDATA_MESSAGE		1001
#define CURTIME_MESSAGE		1002
#define AIRQUALITY_MESSAGE	1003
#define WIFISTATE_MESSAGE	1004
#define PWRSUPPLY_MESSAGE	1005
#define PRC_ORDER_MESSAGE	1006
#define THINGSPEAK_MESSAGE	1007
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

	private:
		bool	active;
		float	temp;
		byte	humidity;
		int		pressure;
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
	COMMON,		// Air Quality (CO, Ammonia, Benzene, Alcohol, smoke) (MQ135)
	H2S,		// сероводород
	CO,			// оксид серы
	SO2,
	CO2,		// углекислый газ
	CH4,
	CH2O,		// формальдегид
	C6H5_CH3,	// толуол
	PM1,		// частицы ~PM1
	PM25,		// частицы ~PM2.5
	VOCs		// formaldehyde benzene concentration
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

/*class PwrSupplyMessage: public IProcessMessage {
	public:
		PwrSupplyMessage(uint32_t startTime): IProcessMessage(NULL, PWRSUPPLY_MESSAGE) {
			this->startTime = startTime;
		}

		uint32_t getStartTime() {
			return this->startTime;
		}

	private:
		uint32_t startTime;
};*/

class ProcessOrderMessage: public IProcessMessage {
	public:
		ProcessOrderMessage(uint16_t lastPid);

		static ProcessOrderMessage* start() {
			return new ProcessOrderMessage(0);
		}

		byte getNextId() {
			return this->nextId;
		}

	private:
		byte nextId;

		void setNextId(byte lastPid, uint16_t processOrderList[], byte len) {
			this->nextId = 0;
			for (byte i = 0; i < len; i++) {
				if (processOrderList[i] == lastPid) {
					if (i == (len - 1)) {
						i = 0;
					}
					this->nextId = processOrderList[i+1];
					return;
				}
			}
		}
};

class ThingspeakFieldMessage: public IProcessMessage {
	public:
		enum ThingspeakChannel {
			CHANNEL1,
			CHANNEL2,
			CHANNEL3
		};

		ThingspeakFieldMessage(ThingspeakChannel channel, byte field, float value): IProcessMessage(NULL, THINGSPEAK_MESSAGE) {
			this->channel = channel;
			this->field = field;
			this->value = value;
		}

		ThingspeakChannel getChannel() {
			return this->channel;
		}

		byte getField() {
			return this->field;
		}

		float getValue() {
			return this->value;
		}

	private:
		ThingspeakChannel channel;
		byte field;
		float value;
};

#endif
