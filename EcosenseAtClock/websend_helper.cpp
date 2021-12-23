#include "websend_helper.h"

UrlParam::UrlParam() {
    this->active = false;
}

void UrlParam::set(/*byte paramId,*/ vType type) {
    //this->paramId = paramId;
    this->valueType = type;
    this->active = true;
}

void UrlParam::setValue(/*byte paramId,*/ uint16_t v) {
    this->ui16 = v;
    set(UINT);
}

void UrlParam::setValue(/*byte paramId,*/ float v) {
    this->f = v;
    set(FLOAT);
}

String UrlParam::getValue() {
    switch (this->valueType)
    {
        //case BYTE: return String(b);
        case UINT: return String(ui16);
        case FLOAT: return String(f);
    }
    return String();
}

bool UrlParam::isActive() {
    return this->active;
}

void ThingspeakWebSendTask::clear() {
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        this->params[i].setValue((uint16_t)0);
        this->params[i].active = false;
    }
    size = 0;
}

String ThingspeakWebSendTask::getUrl(String key) {
    String url;
    url.reserve(14 + key.length() + size * 15);
    url += String(THINGSPEAKREPORT_URI) + key;
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        UrlParam p = this->getParam(i);
        if (p.isActive()) {
            String buf;
            buf.reserve(15);
            buf += F("&field");
            buf += (i + 1);
            buf += F("=");
            buf += p.getValue().substring(0, 5);
            url += buf;
        }
    }
    return url;
}
