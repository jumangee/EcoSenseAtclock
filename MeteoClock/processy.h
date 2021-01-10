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

typedef IFirmwareProcess* (*FactoryFunction)(String, IProcessMessage*);

class ProcessFactoryReg {
	public:
		String id;
		FactoryFunction factory;
		bool isDefault;

		ProcessFactoryReg(String id, FactoryFunction factory, bool isDefault) {
			this->id = id;
			this->factory = factory;
			this->isDefault = isDefault;
		}
};

class IFirmware {
	protected:
		static IFirmware* instance;

		IFirmware();

	public:
		static IFirmware* get() {
			return IFirmware::instance;
		}

		IFirmwareProcess* getProcess(String name);

		virtual void log(String msg) = 0;

		void stopProcess(String name);

		void pauseProcess(String name, unsigned long pauseTime);

		void unPauseProcess(String name);

		void stopAll();

		void soloProcess(String name);

		void sendMessage(IProcessMessage* msg);

		void addProcess(String name);

		void run();

		void addProcess(String name, IProcessMessage* msg);

		void registerFactory(String id, FactoryFunction factory, bool isDefault = false);

	protected:
		LinkedList<IFirmwareProcess*> processList;
		LinkedList<ProcessFactoryReg*> factoryList;
		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}

		void handlerProcessDebugTimer(unsigned long dT);

		void resetProcessMsTotal() {
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = this->processList.get(i);
				process->resetUsedMs();
			}
		}
		#endif

		/*** OVERRIDE THIS! ***/
		bool update(unsigned long ms) {
			return true;
		};
		/*** end: OVERRIDE ***/

		IFirmwareProcess* createProcess(String name, IProcessMessage* msg);

		int findProcess(String name);

		ProcessFactoryReg* findFactoryRegistration(String id);

		ProcessFactoryReg* findDefaultFactoryRegistration();

};

IFirmware* getFirmwareHost();

#endif
