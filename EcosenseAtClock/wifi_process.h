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

class WifiProcess: public IFirmwareProcess {
	private:
		SoftwareSerial 			*espSerial = NULL;
		ThingspeakWebSendTask*	dataSendTask[THINGSPEAK_CHANNELS];

		enum ReportState {
			NONE = 0,
			READY,
			CONNECTED,
			SENT,
			DISABLED
		}						state;
		
	public:
		PROCESSID(PRC_WIFI);

		WifiProcess(IProcessMessage* msg);

		static IFirmwareProcess* factory(IProcessMessage* msg);

		~WifiProcess();

		void espStop();

		void update(unsigned long ms);

		bool WiFiConnect();

		void simpleSendData(ThingspeakWebSendTask *task);

		void sendPacket(String data) {
			EspDrv::sendData(1, data.c_str(), data.length());
		}

		bool handleMessage(IProcessMessage* msg);

		bool dataReadyToSend() {
			for (byte i = 0; i < THINGSPEAK_CHANNELS; i++) {
				if (dataSendTask[i]->size > 0) {
					return true;
				}
			}
			return false;
		}
};

#endif
