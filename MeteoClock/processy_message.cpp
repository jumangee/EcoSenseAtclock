#include "processy_message.h"

String IProcessMessage::getSenderId() {
	if (this->sender)
		return sender->getId();
	return String("");
}

bool IProcessMessage::isSenderId(String & compareTo) {
	if (this->sender)
		return sender->isId(compareTo);
	return false;
}
