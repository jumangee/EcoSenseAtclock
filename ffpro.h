/*
* Firmware Process Control Framework (FFPro)
*
*/
 
#ifndef _FIRMWARE_FRAMEWORK_H
#define _FIRMWARE_FRAMEWORK_H

#include "LinkedList/LinkedList.h"

#include "ffpro_cfg.h"
#include "ffpro_process.h"
#include "ffpro_message.h"
#include <WString.h>
#include <math.h>

typedef IFirmwareProcess* (*FactoryFunction)(String, IFirmware*, IProcessMessage*);

class ProcessFactoryReg {
	public:
		String id;
		FactoryFunction factory;
		bool isDefault;
		
		ProcessFactoryReg(String id, FactoryFunction factory, bool isDefault) {
			id = id;
			factory = factory;
			isDefault = isDefault;
		}
};

class IFirmware {
	protected:
		static IFirmware* instance;

		IFirmware() {
			IFirmware::instance = this;
			
			this->processList = LinkedList<IFirmwareProcess*>(); 
			#ifdef DEBUG_PRO_MS
			this->resetMsDebugTimer(millis());
			#endif
		}
		
	public:
		
		static IFirmware* IFirmware::get() {
			return IFirmware::instance;
		}

		IFirmwareProcess* getProcess(String name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				return this->processList.get(pos);
			}
			return NULL;
		}
				
		virtual void log(String msg) = 0;
			
		void stopProcess(String name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				this->processList.remove(pos);
			}
		}
		
		void pauseProcess(String name, unsigned long pauseTime) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				IFirmwareProcess *process = this->processList.get(pos);
				process->pause(pauseTime);
			}
		}
		
		void unPauseProcess(String name) {
			int pos = this->findProcess(name);
			if (pos > -1) {
				IFirmwareProcess *process = this->processList.get(pos);
				process->unPause();
			}
		}
		
		void stopAll() {
			this->processList.clear();
		}
		
		void soloProcess(String name) {
			this->stopAll();
			this->addProcess(name);
		}
		
		void sendMessage(IProcessMessage* msg) {
			for (int i = 0; i < processList.size(); i++) {
				IFirmwareProcess* process = this->processList.get(i);
				if (process->handleMessage(msg) == true) {	// message processing stop
					return;
				}
			}
		}

		void addProcess(String name) {
			this->addProcess(name, NULL);
		}
		
		void run() {
			if (this->processList.size() == 0) {
				ProcessFactoryReg* registration = this->findDefaultFactoryRegistration();
				if (registration) {
					addProcess(registration->id);	// do something or stop
				} else {
					return;
				}
			}
			
			unsigned long curTime = millis();
			if (this->update(curTime)) {	// true - auto process, false - manual process
				for (int i = 0; i < processList.size(); i++) {
					IFirmwareProcess* process = processList.get(i);
					curTime = process->run(curTime);
				}
			}
			#ifdef DEBUG_PRO_MS
			unsigned long dT = curTime - msDebugTimerStart;
			if (dT > DEBUG_PRO_PERIOD_MS) {
				// call report
				handlerProcessDebugTimer(dT);
			}
			#endif
		}

		void addProcess(String name, IProcessMessage* msg) {
			if (this->findProcess(name) > -1) {
				return;	// only 1 instance of process
			}
			IFirmwareProcess* newProcess = this->createProcess(name, msg);
			if (!newProcess) {
				return;
			}
			
			this->processList.add(newProcess);
		}
		
		void registerFactory(String id, FactoryFunction factory, bool isDefault = false) {
			//if (this->findFactoryRegistration(id) == NULL) {
				factoryList.add(new ProcessFactoryReg(id, factory, isDefault));
			//}
		}
		
	private:
		LinkedList<IFirmwareProcess*> processList;
		LinkedList<ProcessFactoryReg*> factoryList;
		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;
		
		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}
		
		/*** OVERRIDE THIS ***/
		void handlerProcessDebugTimer(unsigned long dT) {
			this->log(">>> dT results >>>");
			unsigned long totalSize = 0;
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = processList.get(i);

				unsigned long spentMs = process->getUsedMs();
				String logMsg = String(process->getId()) + String(": ") + String(round((spentMs * 100) / dT)) + "ms";
				totalSize += sizeof(process);
				this->log(logMsg);
			}
			this->log(String("<<< dT results (") + String(totalSize) + String("bytes) <<<"));
		}
		
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
		
		IFirmwareProcess* createProcess(String name, IProcessMessage* msg) {
			ProcessFactoryReg* factoryReg = findFactoryRegistration(name);
			if (factoryReg) {
				return factoryReg->factory(name, this, msg);
			}
			return NULL;
		}

		int findProcess(String name) {
			for (int i = 0; i < processList.size(); i++) {
				IFirmwareProcess* process = processList.get(i);
				if (process->isId(name)) {
					return i;
				}
			}
			return -1;
		}
		
		ProcessFactoryReg* findFactoryRegistration(String id) {
			for (int i = 0; i < this->factoryList.size(); i++) {
				ProcessFactoryReg* registration = factoryList.get(i);
				if (registration->id == id) {
					return registration;
				}
			}
			return NULL;
		}
		
		ProcessFactoryReg* findDefaultFactoryRegistration() {
			for (int i = 0; i < this->factoryList.size(); i++) {
				ProcessFactoryReg* registration = factoryList.get(i);
				if (registration->isDefault) {
					return registration;
				}
			}
			return NULL;
		}
		
};

IFirmware* getFirmwareHost();

#endif
