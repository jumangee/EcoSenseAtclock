/*
* Processy Firmware Framework
*
*/

#ifndef _FIRMWARE_FRAMEWORK_H
#define _FIRMWARE_FRAMEWORK_H

#include "LinkedList/LinkedList.h"

#include "processy_cfg.h"
#include "processy_process.h"
#include "processy_message.h"
#include <WString.h>
#include <math.h>

typedef IFirmwareProcess* (*ProcessFactory)(String&, IProcessMessage*);

class ProcessFactoryReg {
	public:
		String id;
		ProcessFactory factory;

		ProcessFactoryReg(String & name, ProcessFactory f) {
			this->id = String(name);
			this->factory = f;
		}
};

class IFirmware {
	enum PROCESS {
	};

	protected:
		static IFirmware* instance;

		IFirmware();

	public:
		static IFirmware* get() {
			return IFirmware::instance;
		}

		virtual ProcessFactory getFactory(String & name) = 0;

		IFirmwareProcess* getProcess(String & name);

		void stopProcess(String & name);

		void pauseProcess(String & name, unsigned long pauseTime);

		void unPauseProcess(String & name);

		void stopAll();

		void soloProcess(String & name);

		void sendMessage(IProcessMessage* msg);

		void addProcess(String & name);

		void run();

		void addProcess(String & name, IProcessMessage* msg);

	protected:
		LinkedList<IFirmwareProcess*> processList;

		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}

		void handlerProcessDebugTimer(unsigned long dT);

		void resetProcessMsTotal() {
			for (int i = 0; i < this->processList.size(); i++) {
				//IFirmwareProcess* process = ;
				this->processList.get(i)->resetUsedMs();
			}
		}
		#endif

		bool update(unsigned long ms);

		IFirmwareProcess* createProcess(String & name, IProcessMessage* msg);

		int findProcess(String & name);

};

#endif
