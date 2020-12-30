
#include "ffpro_message.h"

String IProcessMessage::getSenderId() {
	return sender->getId();
}

bool IProcessMessage::isSenderId(String compareTo) {
	return sender->isId(compareTo);
}
