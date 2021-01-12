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

		virtual ProcessFactory getFactory(String & name) = 0;

		//@implement
		IFirmwareProcess* getProcess(String & name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				return this->processList.get(pos);
			}
			return NULL;
		}

		//@implement
		void stopProcess(String & name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				this->processList.remove(pos);
			}
		}

		//@implement
		void pauseProcess(String & name, unsigned long pauseTime) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				//IFirmwareProcess *process = ;
				this->processList.get(pos)->pause(pauseTime);
			}
		}

		//@implement
		void unPauseProcess(String & name) {
			int pos = this->findProcess(name);
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
		void soloProcess(String & name) {
			this->stopAll();
			this->addProcess(name);
		}

		//@implement
		void sendMessage(IProcessMessage* msg) {
			if (msg == NULL) return;

			for (int i = 0; i < processList.size(); i++) {
				//IFirmwareProcess* process = ;
				if (this->processList.get(i)->handleMessage(msg) == true) {	// message processing stop
					break;
				}
			}
			delete msg;
		}

		//@implement
		void addProcess(String & name) {
			this->addProcess(name, NULL);
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
				for (int i = 0; i < this->processList.size(); i++) {
					//IFirmwareProcess* process = ;
					curTime = this->processList.get(i)->run(curTime);
				}
			}
			#ifdef DEBUG_PRO_MS
			unsigned long dT = curTime - msDebugTimerStart;
			if (dT >= DEBUG_PRO_PERIOD_MS) {
				// call report
				handlerProcessDebugTimer(dT);
				this->resetMsDebugTimer(millis());
			}
			#endif
		}

		//@implement
		void addProcess(String & name, IProcessMessage* msg) {
			if (this->findProcess(name) > -1) {
				return;	// only 1 instance of process
			}
			IFirmwareProcess* newProcess = this->createProcess(name, msg);
			if (msg != NULL) {
				delete msg;
			}
			if (newProcess == NULL) {
        		TRACELNF("IFirmware::addProcess//!newProcess")
				return;
			}

      		TRACELNF("IFirmware::addProcess//newProcess")
			this->processList.add(newProcess);
		}

	protected:
		LinkedList<IFirmwareProcess*> processList;

		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}

		//@implement
		//*** OVERRIDE THIS ***/
		//@include "stuff.h"
		//@include "processy_cfg.h"
		//!@include "MemoryFree.h"
		void handlerProcessDebugTimer(unsigned long dT) {
			{
				String s = F("----- PROC SUMMARY (for ");
				s += dT;
				s += F("ms) -----");
				TRACELN(s);
			}
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = processList.get(i);

				unsigned long spentMs = process->getUsedMs();
				{
					String s = String(process->getId());
					s += F(": ");
					s += round((spentMs * 100) / dT);
					s += F("%");
					TRACELN(s);
				}
				process->resetUsedMs();
			}
			//String s = String(F("MEMORY STATUS"));
			//TRACEF("[!] MEMORY STATUS: ");
			//TRACELN(freeMemory())
			//TRACELNF("--------------------------------------");
		}

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
		IFirmwareProcess* createProcess(String & name, IProcessMessage* msg) {
			ProcessFactory factory = this->getFactory(name);
			TRACELNF("IFirmware::createProcess//factory/!")
			if (factory != NULL) {
        		TRACELNF("IFirmware::createProcess//factory")
				IFirmwareProcess* t = factory(name, msg);
				TRACEF("factory state: ")
				if (t == NULL) {
					TRACELNF("ERR")
				}
				else {
					TRACELNF("OK")
				}
				return t;
			}
			return NULL;
		}

		//@implement
		int findProcess(String & name) {
			for (int i = 0; i < this->processList.size(); i++) {
				//IFirmwareProcess* process = this->processList.get(i);
				if (this->processList.get(i)->isId(name)) {
					return i;
				}
			}
			return -1;
		}

};

#endif
