#include "processy.h"

IFirmware* IFirmware::instance = NULL;
#include <Arduino.h>
#include "processy_cfg.h"
#include "stuff.h"
#include "MemoryFree.h"

IFirmware::IFirmware() {
	this->processList = LinkedList<IFirmwareProcess*>();
	#ifdef DEBUG_PRO_MS
	this->resetMsDebugTimer(millis());
	#endif
}

IFirmwareProcess* IFirmware::getProcess(int pId) {
	int pos = this->findProcess(pId);
	if (pos > -1) {
		return this->processList.get(pos);
	}
	return NULL;
}

void IFirmware::stopProcess(int pId) {
	int pos = this->findProcess(pId);
	if (pos > -1) {
		this->processList.remove(pos);
	}
}

void IFirmware::pauseProcess(int pId, unsigned long pauseTime) {
	int pos = this->findProcess(pId);
	if (pos > -1) {
		//IFirmwareProcess *process = ;
		this->processList.get(pos)->pause(pauseTime);
	}
}

void IFirmware::unPauseProcess(int pId) {
	int pos = this->findProcess(pId);
	if (pos > -1) {
		IFirmwareProcess *process = this->processList.get(pos);
		process->unPause();
	}
}

void IFirmware::stopAll() {
	this->processList.clear();
}

void IFirmware::soloProcess(int pId) {
	this->stopAll();
	this->addProcess(pId);
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

void IFirmware::addProcess(int pId) {
	this->addProcess(pId, NULL);
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
		curTime = millis();
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

void IFirmware::addProcess(int pId, IProcessMessage* msg) {
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
      		TRACELNF("IFirmware::addProcess//newProcess")
	this->processList.add(newProcess);
}

		//*** OVERRIDE THIS ***/
void IFirmware::handlerProcessDebugTimer(unsigned long dT) {
	{
		String s = F("----- PROC SUMMARY (for ");
		s += dT;
		s += F("ms) -----");
		TRACELN(s);
	}
	for (int i = 0; i < this->processList.size(); i++) {
		IFirmwareProcess* process = processList.get(i);
		{
			String s = String(process->getId());
			s += F(": ");
			s += round((process->getUsedMs() * 100) / dT);
			s += F("% ");
			TRACELN(s + process->getUsedMs());
		}
		process->resetUsedMs();
	}
	TRACEF("[!] MEMORY STATUS: ");
	{
		int free = freeMemory();
		this->sendMessage(new MemUsageMessage(free));
		TRACELN(free)
	}
	TRACELNF("--------------------------------------");
}

bool IFirmware::update(unsigned long ms) {
	return true;
};

IFirmwareProcess* IFirmware::createProcess(int pId, IProcessMessage* msg) {
	ProcessFactory factory = this->getFactory(pId);
	TRACELNF("IFirmware::createProcess//factory/!")
	if (factory != NULL) {
        		TRACELNF("IFirmware::createProcess//factory")
		IFirmwareProcess* t = factory(pId, msg);
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

int IFirmware::findProcess(int pId) {
	for (int i = 0; i < this->processList.size(); i++) {
		//IFirmwareProcess* process = this->processList.get(i);
		if (this->processList.get(i)->isId(pId)) {
			return i;
		}
	}
	return -1;
}
