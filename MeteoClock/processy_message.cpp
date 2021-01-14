#include "processy_message.h"

int IProcessMessage::getSenderId() {
	if (this->sender)
		return sender->getId();
	return -1;
}

bool IProcessMessage::isSenderId(int compareTo) {
	if (this->sender)
		return sender->isId(compareTo);
	return false;
}
