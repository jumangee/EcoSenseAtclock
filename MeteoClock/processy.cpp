#include "processy.h"

IFirmware* IFirmware::instance = NULL;
#include <Arduino.h>
#include "processy_cfg.h"
#include "stuff.h"

IFirmware::IFirmware() {
	this->processList = LinkedList<IFirmwareProcess*>();
	this->factoryList = LinkedList<ProcessFactoryReg*>();
	#ifdef DEBUG_PRO_MS
	this->resetMsDebugTimer(millis());
	#endif
}

IFirmwareProcess* IFirmware::getProcess(String name) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		return this->processList.get(pos);
	}
	return NULL;
}

void IFirmware::stopProcess(String name) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		this->processList.remove(pos);
	}
}

void IFirmware::pauseProcess(String name, unsigned long pauseTime) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		IFirmwareProcess *process = this->processList.get(pos);
		process->pause(pauseTime);
	}
}

void IFirmware::unPauseProcess(String name) {
	int pos = this->findProcess(name);
	if (pos > -1) {
		IFirmwareProcess *process = this->processList.get(pos);
		process->unPause();
	}
}

void IFirmware::stopAll() {
	this->processList.clear();
}

void IFirmware::soloProcess(String name) {
	this->stopAll();
	this->addProcess(name);
}

void IFirmware::sendMessage(IProcessMessage* msg) {
	for (int i = 0; i < processList.size(); i++) {
		IFirmwareProcess* process = this->processList.get(i);
		if (process->handleMessage(msg) == true) {	// message processing stop
			return;
		}
	}
}

void IFirmware::addProcess(String name) {
	this->addProcess(name, NULL);
}

void IFirmware::run() {
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

void IFirmware::addProcess(String name, IProcessMessage* msg) {
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

void IFirmware::registerFactory(String id, FactoryFunction factory, bool isDefault = false) {
	//if (this->findFactoryRegistration(id) == NULL) {
		factoryList.add(new ProcessFactoryReg(id, factory, isDefault));
	//}
}

		//*** OVERRIDE THIS ***/
void IFirmware::handlerProcessDebugTimer(unsigned long dT) {
	this->log(S(">>> PROC SUMMARY (for ", String(dT).c_str(), "ms)"));
	for (int i = 0; i < this->processList.size(); i++) {
		IFirmwareProcess* process = processList.get(i);
		unsigned long spentMs = process->getUsedMs();
		this->log(S(process->getId().c_str(), ": ", String(round((spentMs * 100) / dT)).c_str(), "%"));
		process->resetUsedMs();
	}
	this->log("end: PROCESS SUMMARY <<<");
}

bool IFirmware::update(unsigned long ms) {
	return true;
};

IFirmwareProcess* IFirmware::createProcess(String name, IProcessMessage* msg) {
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

int IFirmware::findProcess(String name) {
	for (int i = 0; i < this->processList.size(); i++) {
		IFirmwareProcess* process = this->processList.get(i);
		if (process->isId(name)) {
			return i;
		}
	}
	return -1;
}

ProcessFactoryReg* IFirmware::findFactoryRegistration(String id) {
	for (int i = 0; i < this->factoryList.size(); i++) {
		ProcessFactoryReg* registration = this->factoryList.get(i);
		if (registration->id.equals(id)) {
			return registration;
		}
	}
	return NULL;
}

ProcessFactoryReg* IFirmware::findDefaultFactoryRegistration() {
	for (int i = 0; i < this->factoryList.size(); i++) {
		ProcessFactoryReg* registration = this->factoryList.get(i);
		if (registration->isDefault) {
			return registration;
		}
	}
	return NULL;
}
