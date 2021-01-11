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
#include <SafeString.h>
#include <math.h>

typedef IFirmwareProcess* (*FactoryFunction)(String, IProcessMessage*);

class ProcessFactoryReg {
	public:
		String id;
		FactoryFunction factory;
		bool isDefault;

		ProcessFactoryReg(String name, FactoryFunction f, bool isDef) {
			this->id = name;
			this->factory = f;
			this->isDefault = isDef;
		}
};

class IFirmware {
	protected:
		static IFirmware* instance;

		//@implement
		//@include <Arduino.h>
		IFirmware() {
			this->processList = LinkedList<IFirmwareProcess*>();
			this->factoryList = LinkedList<ProcessFactoryReg*>();
			#ifdef DEBUG_PRO_MS
			this->resetMsDebugTimer(millis());
			#endif
		}

	public:
		static IFirmware* get() {
			return IFirmware::instance;
		}

		//@implement
		IFirmwareProcess* getProcess(String name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				return this->processList.get(pos);
			}
			return NULL;
		}

		virtual void log(String msg) = 0;

		//@implement
		void stopProcess(String name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				this->processList.remove(pos);
			}
		}

		//@implement
		void pauseProcess(String name, unsigned long pauseTime) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				IFirmwareProcess *process = this->processList.get(pos);
				process->pause(pauseTime);
			}
		}

		//@implement
		void unPauseProcess(String name) {
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
		void soloProcess(String name) {
			this->stopAll();
			this->addProcess(name);
		}

		//@implement
		void sendMessage(IProcessMessage* msg) {
			for (int i = 0; i < processList.size(); i++) {
				IFirmwareProcess* process = this->processList.get(i);
				if (process->handleMessage(msg) == true) {	// message processing stop
					break;
				}
			}
			delete msg;
		}

		//@implement
		void addProcess(String name) {
			this->addProcess(name, NULL);
		}

		//@implement
		//@include "processy_cfg.h"
		void run() {
			if (this->processList.size() == 0) {
				ProcessFactoryReg* registration = this->findDefaultFactoryRegistration();
				if (registration) {
					this->addProcess(registration->id);	// do something or stop
				} else {
					TRACE("NOTHING TO DO!")
					return;
				}
			}
			unsigned long curTime = millis();
			if (this->update(curTime)) {	// true - auto process, false - manual process
				for (int i = 0; i < this->processList.size(); i++) {
					IFirmwareProcess* process = this->processList.get(i);
					curTime = process->run(curTime);
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
		void addProcess(String name, IProcessMessage* msg) {
      		//TRACE("IFirmware::addProcess//1")
			if (this->findProcess(name) > -1) {
				return;	// only 1 instance of process
			}
      		//TRACE("IFirmware::addProcess//2")
			IFirmwareProcess* newProcess = this->createProcess(name, msg);
			if (newProcess == NULL) {
        		TRACE("IFirmware::addProcess//!newProcess")
				return;
			}

      		TRACE("IFirmware::addProcess//newProcess")
			this->processList.add(newProcess);
		}

		//@implement
		//@include "stuff.h"
		void registerFactory(String id, FactoryFunction factory, bool isDefault = false) {
			//if (this->findFactoryRegistration(id) == NULL) {
				factoryList.add(new ProcessFactoryReg(id, factory, isDefault));
			//}
		}

	protected:
		LinkedList<IFirmwareProcess*> processList;
		//LinkedList<ProcessFactoryReg> factoryList;
		ProcessFactoryReg[] *factoryList;
		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}

		//@implement
		//*** OVERRIDE THIS ***/
		//@include "stuff.h"
		//@include "processy_cfg.h"
		void handlerProcessDebugTimer(unsigned long dT) {
			this->log(S(">>> PROC SUMMARY (for ", String(dT).c_str(), "ms)"));
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = processList.get(i);

				unsigned long spentMs = process->getUsedMs();
				this->log(S(process->getId().c_str(), ": ", String(round((spentMs * 100) / dT)).c_str(), "%"));
				process->resetUsedMs();
			}
			this->log("end: PROCESS SUMMARY <<<");
		}

		void resetProcessMsTotal() {
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = this->processList.get(i);
				process->resetUsedMs();
			}
		}
		#endif

		//@implement
		bool update(unsigned long ms) {
			return true;
		};

		//@implement
		IFirmwareProcess* createProcess(String name, IProcessMessage* msg) {
			TRACE( S("IFirmware::createProcess::findFactoryRegistration: ", name.c_str()) );
			ProcessFactoryReg* factoryReg = this->findFactoryRegistration(name);
			TRACE("IFirmware::createProcess//factory/!")
			if (factoryReg != NULL) {
        		TRACE("IFirmware::createProcess//factory")
				IFirmwareProcess* t = factoryReg->factory(name, msg);
				TRACE("factory state:")
				if (t == NULL) {
					TRACE("factory ERR")
				}
				else {
					TRACE("factory OK")
				}
				return t;
			}
      		TRACE(S("IFirmware::createProcess//!factoryReg:", name.c_str()))
			return NULL;
		}

		//@implement
		int findProcess(String name) {
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = this->processList.get(i);
				if (process->isId(name)) {
					return i;
				}
			}
			return -1;
		}

		//@implement
		//@include "stuff.h"
		ProcessFactoryReg* findFactoryRegistration(String id) {
			for (int i = 0; i < this->factoryList.size(); i++) {
				ProcessFactoryReg* registration = this->factoryList.get(i);
				if (registration->id.equals(id)) {
					return registration;
				}
			}
			return NULL;
		}

		//@implement
		ProcessFactoryReg* findDefaultFactoryRegistration() {
			for (int i = 0; i < this->factoryList.size(); i++) {
				ProcessFactoryReg* registration = this->factoryList.get(i);
				if (registration->isDefault) {
					return registration;
				}
			}
			return NULL;
		}

};

#endif
