#ifndef _ECOSENSEATCLOCK_MESSAGES_H
#define _ECOSENSEATCLOCK_MESSAGES_H

class IFirmwareProcess;

#include "processy_message.h"
#include <Arduino.h>
#include <math.h>

#include "ecosense_cfg.h"

//--- MSG TYPES -------------------
#define ENVDATA_MESSAGE		1001
#define CURTIME_MESSAGE		1002
#define AIRQUALITY_MESSAGE	1003
#define PWRSUPPLY_MESSAGE	1005
#define PRC_ORDER_MESSAGE	1006
//#define THINGSPEAK_MESSAGE	1007
#define TASKDONE_MESSAGE	1008
//---------------------------------

class EnvDataMessage: public IProcessMessage {
	public:
		EnvDataMessage(float t, float h, uint16_t p): IProcessMessage(NULL, ENVDATA_MESSAGE) {
			this->temp =  t;
			this->humidity = h;
			this->pressure = p;
		}

		float getTemp() {
			return this->temp;
		}

		float getHumidity() {
			return this->humidity;
		}

		uint16_t	getPressure() {
			return this->pressure;
		}

	private:
		float		temp;
		float		humidity;
		uint16_t	pressure;
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
				time += '0'; //String(F("0"));
			}
			time += String(getHrs());
			time += getDots() ? ':' : ' '; //F(":") : F(" ");
			if (getMins() < 10) {
				time += '0'; //String(F("0"));
			}
			time += getMins();
			return time;
		}

	private:
		int8_t hrs, mins, secs;
        boolean dotFlag;
};

class AirQualityMsg: public IProcessMessage {
	public:
		enum GasType {
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
		
		enum GasConcentration {
			MINIMAL,
			NORM,
			WARNING,
			DANGER
		};
		
		/**
		 * gasType: measured type
		 * ////quality: 0-42 - norm, 43-84 - high, 84-127 - danger
		 * quality: 0 - none, 1 - norm, 2 - medium, 3 - high, 4 - danger
		 */
		AirQualityMsg(GasType gasType, GasConcentration concentration, uint16_t amt): IProcessMessage(NULL, AIRQUALITY_MESSAGE) {
			this->gas =  gasType;
			this->concentration = concentration;
			this->amount = amt;
		}

		GasType gasType() {
			return this->gas;
		}

		GasConcentration getConcentration() {
			return this->concentration;
		}

		uint16_t getAmount() {
			return this->amount;
		}

		static GasConcentration value2code(uint8_t v) {
			return static_cast<GasConcentration>(v);
		}

	private:
		GasType				gas;
		GasConcentration	concentration;
		uint16_t			amount;
};


class ProcessOrderMessage: public IProcessMessage {
	public:
		static ProcessOrderMessage* start() {
			return new ProcessOrderMessage();
		}

		static ProcessOrderMessage* goNextOf(uint16_t currentId) {
			return new ProcessOrderMessage(currentId);
		}

		uint16_t getNextId() {
			return this->nextId;
		}

	private:
		uint16_t nextId;

		/**
		 * @brief Definition of process start order
		 */
		uint16_t processOrderList[3] = {PRC_CONSUMER1, PRC_CONSUMER2, PRC_CONSUMER3};

		//@implement
		ProcessOrderMessage(const uint16_t lastPid = 0): IProcessMessage(NULL, PRC_ORDER_MESSAGE) {
			byte len = sizeof(this->processOrderList)/sizeof(this->processOrderList[0]);

			byte pos = 0;
			if (lastPid != 0) {
				for (byte i = 1; i <= len; i++) {
					if (lastPid == this->processOrderList[i-1]) {
						if (i == len) {
							pos = 0;	// restart list
						} else {
							pos = i;
						}
						break;
					}
				}
			}

			this->nextId = this->processOrderList[pos];
			return;
		}
};

/*class ThingspeakFieldMessage: public IProcessMessage {
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
};*/

class TaskDoneMessage: public IProcessMessage {
	public:
		TaskDoneMessage(IFirmwareProcess* prc): IProcessMessage(NULL, TASKDONE_MESSAGE) {
			this->taskId = prc->getId();
		}

		uint16_t getTaskId() {
			return this->taskId;
		}

	private:
		uint16_t	taskId;
};

#endif
