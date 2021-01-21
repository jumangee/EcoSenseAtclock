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

		//@implement
		//@include <Arduino.h>
		IFirmware() {
			this->processList = LinkedList<IFirmwareProcess*>();

			#ifdef DEBUG_PRO_MS
			this->resetMsDebugTimer(millis());
			#endif
		}

	public:
		static IFirmware* get() {
			return IFirmware::instance;
		}

		virtual ProcessFactory getFactory(int pId) = 0;

		//@implement
		IFirmwareProcess* getProcess(int pId) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				return this->processList.get(pos);
			}
			return NULL;
		}

		//@implement
		void stopProcess(int pId) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				this->processList.get(pos)->stop();
			}
		}

		//@implement
		void pauseProcess(int pId, unsigned long pauseTime) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				//IFirmwareProcess *process = ;
				this->processList.get(pos)->pause(pauseTime);
			}
		}

		//@implement
		void unPauseProcess(int pId) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				IFirmwareProcess *process = this->processList.get(pos);
				process->unPause();
			}
		}

		//@implement
		void stopAll() {
			this->processList.clear();
		}

		//@implement
		void soloProcess(int pId) {
			this->stopAll();
			this->addProcess(pId);
		}

		//@implement
		void sendMessage(IProcessMessage* msg) {
			if (msg == NULL) return;

			for (int i = 0; i < processList.size(); i++) {
				if (this->processList.get(i)->handleMessage(msg) == true) {	// message processing stop
					break;
				}
			}
			delete msg;
		}

		//@implement
		void addProcess(int pId) {
			this->addProcess(pId, NULL);
		}

		//@implement
		//@include "processy_cfg.h"
		void run() {
			if (this->processList.size() == 0) {
				#ifdef DEFAULT_PROCESS
					this->addProcess(F(DEFAULT_PROCESS));
				#else
					TRACELNF("NOTHING TO DO!")
					return;
				#endif
			}
			unsigned long curTime = millis();
			if (this->update(curTime)) {	// true - auto process, false - manual process
				curTime = millis();
				for (int i = 0; i < this->processList.size(); i++) {
					IFirmwareProcess *p = this->processList.get(i);
					if (p->getState() != IFirmwareProcess::ProcessState::STOP) {
						curTime = p->run(curTime);
					}
				}

				// safely kill stopped processes
				for (int i = this->processList.size()-1; i >= 0; i--) {
					if (this->processList.get(i)->getState() == IFirmwareProcess::ProcessState::STOP) {
						this->processList.remove(i);
					}
				}
			}
			#ifdef DEBUG_PRO_MS
			unsigned long dT = curTime - msDebugTimerStart;
			if (dT >= DEBUG_PRO_PERIOD_MS) {
				// call report
				handlerProcessDebugTimer(dT);
				this->resetMsDebugTimer(millis());
			}
			#else
				handlerProcessDebugTimer(0);
			#endif
		}

		//@implement
		void addProcess(int pId, IProcessMessage* msg) {
			if (this->findProcess(pId) > -1) {
				return;	// only 1 instance of process
			}
			IFirmwareProcess* newProcess = this->createProcess(pId, msg);
			if (msg != NULL) {
				delete msg;
			}
			if (newProcess == NULL) {
        		TRACELNF("IFirmware::addProcess//!newProcess")
				return;
			}

			this->processList.add(newProcess);
		}

	protected:
		LinkedList<IFirmwareProcess*> processList;

		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}
		#endif

		//@implement
		//*** OVERRIDE THIS ***/
		//@include "stuff.h"
		//--@include "processy_cfg.h"
		//@include "MemoryFree.h"
		void handlerProcessDebugTimer(unsigned long dT) {
			#ifdef DEBUG_PRO_MS
			{
				TRACEF("----- PROC SUMMARY (for ");
				TRACE(dT);
				TRACELNF("ms) -----");
			}
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = processList.get(i);
				{
					TRACE(process->getId());
					TRACEF(": ");
					TRACE(round((process->getUsedMs() * 100) / dT));
					TRACE(process->getUsedMs());
					TRACELNF("% ");
				}
				process->resetUsedMs();
			}
			#endif
			TRACEF("[!] MEMORY STATUS: ");
			{
				int free = freeMemory();
				this->sendMessage(new MemUsageMessage(free));
				TRACELN(free)
			}
			TRACELNF("--------------------------------------");
		}

		#ifdef DEBUG_PRO_MS
		void resetProcessMsTotal() {
			for (int i = 0; i < this->processList.size(); i++) {
				//IFirmwareProcess* process = ;
				this->processList.get(i)->resetUsedMs();
			}
		}
		#endif

		//@implement
		bool update(unsigned long ms) {
			return true;
		};

		//@implement
		IFirmwareProcess* createProcess(int pId, IProcessMessage* msg) {
			ProcessFactory factory = this->getFactory(pId);
			TRACELNF("IFirmware::createProcess//factory/!")
			if (factory != NULL) {
        		TRACELNF("IFirmware::createProcess//factory")
				IFirmwareProcess* t = factory(pId, msg);
				return t;
			}
			return NULL;
		}

		//@implement
		int findProcess(int pId) {
			for (int i = 0; i < this->processList.size(); i++) {
				if (this->processList.get(i)->isId(pId)) {
					return i;
				}
			}
			return -1;
		}

};

#endif
