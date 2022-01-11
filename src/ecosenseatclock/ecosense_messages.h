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
#define SELFREPORT_MESSAGE	1007
#define TASKDONE_MESSAGE	1008
#define BTNCLICK_MESSAGE	1009
#define WIFISTATE_MESSAGE	1010
//---------------------------------

class EnvDataMessage: public IProcessMessage {
	public:
		EnvDataMessage(float t, float h, uint16_t p): IProcessMessage(NULL, ENVDATA_MESSAGE) {
			this->temp =  t;
			this->humidity = h;
			this->pressure = p;
		}

		float		temp;
		float		humidity;
		uint16_t	pressure;
};

class WifiStateMessage: public IProcessMessage {
	public:
		bool online = false;
		WifiStateMessage(bool state): IProcessMessage(NULL, WIFISTATE_MESSAGE) {
			online = state;
		}
};

class CurrentTimeMsg: public IProcessMessage {
	public:
		CurrentTimeMsg(int8_t hrs, int8_t mins): IProcessMessage(NULL, CURTIME_MESSAGE) {
			this->hrs =  hrs;
			this->mins = mins;
			
			/*TRACEF("Time: ")
			TRACE(hrs);
			TRACEF(":")
			TRACELN(mins)*/
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


#define BUTTON_EVENT ButtonClickMessage::ButtonEvent

class ButtonClickMessage: public IProcessMessage {
	public:
		enum ButtonEvent {
			CLICK,
			HOLD,
			DBLCLICK
		} event;

		uint8_t btnId;

		ButtonClickMessage(uint8_t id, ButtonEvent e = CLICK): IProcessMessage(NULL, BTNCLICK_MESSAGE) {
			event = e;
			btnId = id;
		}
};


class SelfReportMessage: public IProcessMessage {
	public:
		SelfReportMessage(byte p, uint16_t m): IProcessMessage(NULL, SELFREPORT_MESSAGE) {
			processCount = p;
			freemem = m;
		}

		byte processCount;
		uint16_t freemem;
};

#endif
