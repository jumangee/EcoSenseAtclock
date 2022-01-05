/**
 * Processy Firmware Framework Core
 */

#ifndef _FIRMWARE_FRAMEWORK_H
#define _FIRMWARE_FRAMEWORK_H

#include "processy_cfg.h"
#include "processy_process.h"
#include "processy_message.h"
#include <math.h>

typedef IFirmwareProcess* (*ProcessFactory)(IProcessMessage*);

#define PROCESS_REG(className) registerProcess(className::ID, &className::factory)

struct IFirmwareProcessRegistration {
	public:
		uint16_t 			id;
		ProcessFactory		factory;
		IFirmwareProcess*	prc;

		IFirmwareProcessRegistration(uint16_t pId, ProcessFactory f) {
			id = pId;
			factory = f;
			prc = NULL;
		}

		void kill() {
			if (prc != NULL) {
				delete this->prc;
				this->prc = NULL;
			}
		}

		bool isActive() {
			return prc != NULL;
		}
};

class IFirmware {
	protected:
		static IFirmware*				instance;
		IFirmwareProcessRegistration*	processList[PROCESSY_MAX_LIST];
		uint16_t						processListSize = 0;

		IFirmware() {
			#ifdef DEBUG_PRO_MS
			this->resetMsDebugTimer(millis());
			#endif
		}

	public:
		static IFirmware* get() {
			return IFirmware::instance;
		}

		IFirmwareProcess* getProcess(uint16_t pId);

		void stopProcess(uint16_t pId);

		void pauseProcess(uint16_t pId, unsigned long pauseTime);

		void unPauseProcess(uint16_t pId);

		void stopAll();

		void soloProcess(uint16_t pId);

		void sendMessage(IProcessMessage* msg);

		void run();

		void addProcess(uint16_t pId, IProcessMessage* msg = NULL);

	protected:
		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}
		#endif

		virtual void handlerProcessDebugTimer(unsigned long dT);

		bool update(unsigned long ms);

		int findProcess(uint16_t pId);

		IFirmwareProcessRegistration* findRegistration(uint16_t pId);

		void registerProcess(uint16_t pId, ProcessFactory f);

};

#endif
