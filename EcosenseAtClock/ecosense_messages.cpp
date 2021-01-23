#include "ecosense_messages.h"
#include "ecosenseatclock.h"

ProcessOrderMessage::ProcessOrderMessage(uint16_t lastPid) : IProcessMessage(NULL, PRC_ORDER_MESSAGE){
	/**
	 * process order defined here
	 */
	const static uint16_t processOrderList[] = {0, PRC_CONSUMER1, PRC_CONSUMER2, PRC_CONSUMER3};
	
	this->setNextId(lastPid, processOrderList, *(&processOrderList + 1) - processOrderList);
	TRACEF("ProcessOrderMessage/nextId=")
	TRACELN(this->nextId)
}
