#include "processy.h"

IFirmware* IFirmware::instance = NULL;
#include <Arduino.h>
#include "processy_cfg.h"
#include "stuff.h"

IFirmware::IFirmware() {
	this->processList = LinkedList<IFirmwareProcess*>();
	#ifdef DEBUG_PRO_MS
	this->resetMsDebugTimer(millis());
	#endif
}

IFirmwareProcess* IFirmware::getProcess(String & name) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		return this->processList.get(pos);
	}
	return NULL;
}

void IFirmware::stopProcess(String & name) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		this->processList.remove(pos);
	}
}

void IFirmware::pauseProcess(String & name, unsigned long pauseTime) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		//IFirmwareProcess *process = ;
		this->processList.get(pos)->pause(pauseTime);
	}
}

void IFirmware::unPauseProcess(String & name) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		IFirmwareProcess *process = this->processList.get(pos);
		process->unPause();
	}
}

void IFirmware::stopAll() {
	this->processList.clear();
}

void IFirmware::soloProcess(String & name) {
	this->stopAll();
	this->addProcess(name);
}

void IFirmware::sendMessage(IProcessMessage* msg) {
	if (msg == NULL) return;
	for (int i = 0; i < processList.size(); i++) {
		//IFirmwareProcess* process = ;
		if (this->processList.get(i)->handleMessage(msg) == true) {	// message processing stop
			break;
		}
	}
	delete msg;
}

void IFirmware::addProcess(String & name) {
	this->addProcess(name, NULL);
}

void IFirmware::run() {
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

void IFirmware::addProcess(String & name, IProcessMessage* msg) {
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

		//*** OVERRIDE THIS ***/
		//!@include "MemoryFree.h"
void IFirmware::handlerProcessDebugTimer(unsigned long dT) {
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

bool IFirmware::update(unsigned long ms) {
	return true;
};

IFirmwareProcess* IFirmware::createProcess(String & name, IProcessMessage* msg) {
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

int IFirmware::findProcess(String & name) {
	for (int i = 0; i < this->processList.size(); i++) {
		//IFirmwareProcess* process = this->processList.get(i);
		if (this->processList.get(i)->isId(name)) {
			return i;
		}
	}
	return -1;
}
