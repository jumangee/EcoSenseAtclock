#include "websend_helper.h"

void UrlParam::set(vType type) {
    this->valueType = type;
    //this->active = true;
}

void UrlParam::setValue(uint16_t v) {
    this->ui16 = v;
    set(UINT);
}

void UrlParam::setValue(float v) {
    this->f = v;
    set(FLOAT);
}

String UrlParam::getValue() {
    return (this->valueType == UINT) ? String(ui16) : String(f);
}

void ThingspeakWebSendTask::clear() {
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        if (isParam(i)) {
            delete this->params[i];
        }
        this->params[i] = NULL;
    }
    size = 0;
}

const __FlashStringHelper* ThingspeakWebSendTask::getApiKey() {
    return apiKey;
}
