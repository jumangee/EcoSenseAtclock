#include "processy.h"

IFirmware* IFirmware::instance = NULL;
#include "processy_cfg.h"
#include "stuff.h"
#include "MemoryFree.h"

IFirmwareProcess* IFirmware::getProcess(uint16_t pId) {
	int pos = this->findProcess(pId);
	return pos > -1 ? this->processList[pos]->prc : NULL;
}

void IFirmware::stopProcess(uint16_t pId) {
	IFirmwareProcessRegistration* reg = this->findRegistration(pId);
	if (reg != NULL && reg->isActive()) {
		reg->prc->stop();
	}
}

void IFirmware::pauseProcess(uint16_t pId, unsigned long pauseTime) {
	IFirmwareProcessRegistration* reg = this->findRegistration(pId);
	if (reg != NULL) {
		reg->prc->pause(pauseTime);
	}
}

void IFirmware::unPauseProcess(uint16_t pId) {
	IFirmwareProcessRegistration* reg = this->findRegistration(pId);
	if (reg != NULL) {
		reg->prc->unPause();
	}
}

void IFirmware::stopAll() {
	for (uint16_t i = 0; i < this->processListSize; i++) {
		this->processList[i]->kill();
	}
}

void IFirmware::soloProcess(uint16_t pId) {
	this->stopAll();
	this->addProcess(pId);
}

void IFirmware::sendMessage(IProcessMessage* msg) {
	if (msg == NULL) return;
	for (uint16_t i = 0; i < processListSize; i++) {
		IFirmwareProcessRegistration* reg = this->processList[i];
		if (reg->isActive()) {	// message processing stop
			if (reg->prc->handleMessage(msg) == true) {
				break;
			}
		}
	}
	delete msg;
}

void IFirmware::run() {
	unsigned long curTime = millis();
	if (this->update(curTime)) {	// true - auto process, false - manual process
		curTime = millis();
		for (uint16_t i = 0; i < processListSize; i++) {
			IFirmwareProcessRegistration* reg = this->processList[i];
			if (reg->isActive()) {
				if (reg->prc->getState() == IFirmwareProcess::ProcessState::STOP) {
					reg->kill();
				} else if (!reg->prc->isPaused(curTime)) {
					curTime = reg->prc->run(curTime);
				}
			}
		}
		// safely kill stopped processes
		/*for (uint16_t i = 0; i < this->processListSize; i++) {
			IFirmwareProcessRegistration* reg = this->processList[i];
			if (reg->isActive() && reg->prc->getState() == IFirmwareProcess::ProcessState::STOP) {
				reg->kill();
			}
		}*/
	}
	#ifdef DEBUG_PRO_MS > 0
	unsigned long dT = curTime - msDebugTimerStart;
	if (dT >= DEBUG_PRO_PERIOD_MS) {
		// call report
		handlerProcessDebugTimer(dT);
		this->resetMsDebugTimer(millis());
	}
	#endif
}

void IFirmware::addProcess(uint16_t pId, IProcessMessage* msg = NULL) {
	int i = findProcess(pId);
	if (i > -1) {
		IFirmwareProcessRegistration* reg = this->processList[i];
		if (!reg->isActive()) {
			reg->prc = reg->factory(msg);
			if (msg != NULL) {
				delete msg;
			}
		}
	}
}

void IFirmware::handlerProcessDebugTimer(unsigned long dT) {
	#if DEBUG_PRO_MS == 1
		#if PROCESSY_DEBUG_SERIAL == 1
		TRACEF("----- PROC SUMMARY (for ");
		TRACE(dT);
		TRACELNF("ms) -----");
		for (uint16_t i = 0; i < this->processListSize; i++) {
			IFirmwareProcessRegistration* reg = this->processList[i];
			if (reg->isActive()) {
				uint32_t used = reg->prc->getUsedMs();
				TRACE(reg->id)
				TRACEF(": ")
				TRACE(used)
				TRACEF("ms (");
				TRACE(round((used * 100) / dT))
				TRACELNF("%)");
				reg->prc->resetUsedMs();
			}
		}
		#endif
	int free = freeMemory();
	this->sendMessage(new MemUsageMessage(free));
	TRACEF("MEM FREE:");
	TRACELN(free)
	TRACELNF("--------------------------------------");
	#endif
}

bool IFirmware::update(unsigned long ms) {
	return true;
};

int IFirmware::findProcess(uint16_t pId) {
	for (uint16_t i = 0; i < processListSize; i++) {
		if (this->processList[i]->id == pId) {
			return i;
		}
	}
	return -1;
}

IFirmwareProcessRegistration* IFirmware::findRegistration(uint16_t pId) {
	for (uint16_t i = 0; i < processListSize; i++) {
		if (this->processList[i]->id == pId) {
			return this->processList[i];
		}
	}
	return NULL;
}

void IFirmware::registerProcess(uint16_t pId, ProcessFactory f) {
	this->processList[this->processListSize] = new IFirmwareProcessRegistration(pId, f);
	this->processListSize++;
}
