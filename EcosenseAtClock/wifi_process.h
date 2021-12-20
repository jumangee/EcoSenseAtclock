/**
 * WiFi connectivity and ThingSpeak report process
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_WIFI_H
#define _ECOSENSEATCLOCK_WIFI_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"

//#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EspDrv/EspDrv.h"

#define _ESPLOGLEVEL_ 0

#include "ecosense_cfg.h"
#include "ecosense_messages.h"
#include "websend_helper.h"

//#define TIMEOUT 5000 // mS

class WifiProcess: public IFirmwareProcess {
	private:
		bool ready;
		SoftwareSerial *espSerial;
		unsigned long lastReportTime;
		ThingspeakWebSendTask dataSendTask;
	public:
		PROCESSID(PRC_WIFI);

		WifiProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		~WifiProcess();

		void update(unsigned long ms);

		bool WiFiConnect();

		void WiFiDisconnect();

		void simpleSendData();

		bool handleMessage(IProcessMessage* msg);

		void handleAirQualityMsg(AirQualityMsg* msg);

};

#endif
