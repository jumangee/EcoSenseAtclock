#include "websend_helper.h"

UrlParam::UrlParam() {
    this->active = false;
}

void UrlParam::set(vType type) {
    this->valueType = type;
    this->active = true;
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
    switch (this->valueType)
    {
        //case BYTE: return String(b);
        case UINT: return String(ui16);
    }
    return String(f);
}

void ThingspeakWebSendTask::clear() {
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        //this->params[i].setValue((uint16_t)0);
        this->params[i].active = false;
    }
    size = 0;
}

String ThingspeakWebSendTask::getUrl() {
    String url;
    url.reserve(size * 15);
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        UrlParam p = this->getParam(i);
        if (p.active) {
            String buf;
            buf.reserve(15);
            buf += F("&field");
            buf += (i + 1);
            buf += F("=");
            buf += p.getValue(); //.substring(0, 8);
            url += buf;
        }
    }
    return url;
}
