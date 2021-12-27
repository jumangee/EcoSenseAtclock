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
//#define PRC_ORDER_MESSAGE	1006
//#define THINGSPEAK_MESSAGE	1007
#define TASKDONE_MESSAGE	1008
#define BTNCLICK_MESSAGE	1009
#define WIFIEVENT_MESSAGE	1010
#define PPD42NS_MESSAGE		1011
//---------------------------------

class WifiEventMessage: public IProcessMessage {
	public:
		enum WifiEvent{
			OK,
			ERROR,
			NONE
		} event;

		WifiEventMessage(WifiEvent e): IProcessMessage(NULL, WIFIEVENT_MESSAGE) {
			this->event = e;
		}
};


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

		uint16_t getPressure() {
			return this->pressure;
		}

	private:
		float		temp;
		float		humidity;
		uint16_t	pressure;
};


class CurrentTimeMsg: public IProcessMessage {
	public:
		CurrentTimeMsg(int8_t hrs, int8_t mins): IProcessMessage(NULL, CURTIME_MESSAGE) {
			this->hrs =  hrs;
			this->mins = mins;
			
			TRACEF("Time: ")
			TRACE(hrs);
			TRACEF(":")
			TRACELN(mins)
		}

		int8_t getHrs() {
			return this->hrs;
		}

		int8_t getMins() {
			return this->mins;
		}

	private:
		int8_t hrs, mins;
        //boolean dotFlag;
};

class AirQualityMsg: public IProcessMessage {
	public:
		enum GasType {
			COMMON,		//0		// Air Quality (CO, Ammonia, Benzene, Alcohol, smoke) (MQ135)
			H2S,		//1		// сероводород
			CO,			//2		// оксид серы
			SO2,		//3
			CO2,		//4		// углекислый газ
			CH4,		//5
			CH2O,		//6		// формальдегид
			C6H5_CH3,	//7		// толуол
			PM1,		//8		// частицы ~PM1
			PM25,		//9		// частицы ~PM2.5
			VOCs		//10	// formaldehyde benzene concentration
		};
		
		enum GasConcentration {
			MINIMAL,
			NORM,
			WARNING,
			DANGER
		};
		
		/**
		 * gasType: measured gas type
		 * quality: 0 - none, 1 - norm, 2 - medium, 3 - high, 4 - danger
		 */
		AirQualityMsg(GasType gasType, GasConcentration concentration, float amt): IProcessMessage(NULL, AIRQUALITY_MESSAGE) {
			this->gas =  gasType;
			this->concentration = concentration;
			this->amount = amt;

			TRACEF("AirQualityMsg: gas=")
			TRACE(int(this->gas))
			TRACEF(", conc=")
			TRACE(int(concentration))
			TRACEF(", amt=")
			TRACELN(amt)
		}

		GasType gasType() {
			return this->gas;
		}

		GasConcentration getConcentration() {
			return this->concentration;
		}

		float getAmount() {
			return this->amount;
		}

		static GasConcentration value2code(uint8_t v) {
			return static_cast<GasConcentration>(v);
		}

	private:
		GasType				gas;
		GasConcentration	concentration;
		float				amount;
};


/*class ProcessOrderMessage: public IProcessMessage {
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

		uint16_t processOrderList[3] = {PRC_CONSUMER1, PRC_CONSUMER2, PRC_CONSUMER3};

		ProcessOrderMessage(const uint16_t lastPid = 0): IProcessMessage(NULL, PRC_ORDER_MESSAGE) {
			byte len = sizeof(this->processOrderList)/sizeof(this->processOrderList[0]);

			byte pos = 0;
			if (lastPid != 0) {
				for (byte i = 1; i <= len; i++) {
					if (lastPid == this->processOrderList[i-1]) {
						if (i < len) {
							pos = i;
						} else {
							pos = 0;	// restart list
						}
						break;
					}
				}
			}

			this->nextId = this->processOrderList[pos];
			return;
		}
};*/

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
		TaskDoneMessage(IFirmwareProcess* prc): IProcessMessage(prc, TASKDONE_MESSAGE) {
			TRACEF("TaskDone: ")
			TRACELN(this->getTaskId())
		}

		uint16_t getTaskId() {
			return getSender()->getId();
		}
};

/** SINGLE BTN SUPPORT FOR THIS PROJECT
 */
class ButtonClickMessage: public IProcessMessage {
	public:
		enum ButtonEvent {
			CLICK,
			HOLD
		} event;

		ButtonClickMessage(ButtonEvent e = CLICK): IProcessMessage(NULL, BTNCLICK_MESSAGE) {
			this->event = e;
		}
};


/*class ParticlePPD42Message: public IProcessMessage {
	public:
		ParticlePPD42Message(float ratioPm1, float concentrationPm1, float ratioPm2, float concentrationPm2): IProcessMessage(NULL, PPD42NS_MESSAGE) {
			this->ratioPm1 = ratioPm1;
			this->ratioPm2 = ratioPm2;
			this->concentrationPm1 = concentrationPm1;
			this->concentrationPm2 = concentrationPm2;
		}

		float ratioPm1;
		float ratioPm2;
		float concentrationPm1;
		float concentrationPm2;
};*/

#endif
