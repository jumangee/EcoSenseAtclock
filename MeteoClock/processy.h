/**
 * Processy Firmware Framework Core
 */

#ifndef _FIRMWARE_FRAMEWORK_H
#define _FIRMWARE_FRAMEWORK_H

#include "LinkedList/LinkedList.h"

#include "processy_cfg.h"
#include "processy_process.h"
#include "processy_message.h"
#include <math.h>

#define FACTORY(name, className) ProcessFactoryReg(name, &className::factory)

typedef IFirmwareProcess* (*ProcessFactory)(int, IProcessMessage*);

class ProcessFactoryReg {
	public:
		int id;
		ProcessFactory factory;

		ProcessFactoryReg(int pId, ProcessFactory f) {
			this->id = pId;
			this->factory = f;
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

		virtual ProcessFactory getFactory(int pId) = 0;

		IFirmwareProcess* getProcess(int pId);

		void stopProcess(int pId);

		void pauseProcess(int pId, unsigned long pauseTime);

		void unPauseProcess(int pId);

		void stopAll();

		void soloProcess(int pId);

		void sendMessage(IProcessMessage* msg);

		void addProcess(int pId);

		void run();

		void addProcess(int pId, IProcessMessage* msg);

	protected:
		LinkedList<IFirmwareProcess*> processList;

		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}
		#endif

		void handlerProcessDebugTimer(unsigned long dT);

		#ifdef DEBUG_PRO_MS
		void resetProcessMsTotal() {
			for (int i = 0; i < this->processList.size(); i++) {
				//IFirmwareProcess* process = ;
				this->processList.get(i)->resetUsedMs();
			}
		}
		#endif

		bool update(unsigned long ms);

		IFirmwareProcess* createProcess(int pId, IProcessMessage* msg);

		int findProcess(int pId);

};

#endif
