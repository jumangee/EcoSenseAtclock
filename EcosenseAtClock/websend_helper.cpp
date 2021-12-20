#include "websend_helper.h"

UrlParam::UrlParam() {
    this->active = false;
}

byte UrlParam::getId() {
    return this->paramId;
}

void UrlParam::setId(byte paramId, vType type) {
    this->paramId = paramId;
    this->valueType = type;
    this->setActive(true);
}

void UrlParam::setValue(byte paramId, byte v) {
    this->b = v;
    setId(paramId, BYTE);
}

void UrlParam::setValue(byte paramId, uint16_t v) {
    this->ui16 = v;
    setId(paramId, UINT);
}

void UrlParam::setValue(byte paramId, float v) {
    this->f = v;
    setId(paramId, FLOAT);
}

String UrlParam::getValue() {
    switch (this->valueType)
    {
        case BYTE: return String(b);
        case UINT: return String(ui16);
        case FLOAT: return String(f);
    }
    return String();
}

bool UrlParam::isActive() {
    return this->active;
}

void UrlParam::setActive(bool s) {
    this->active = s;
}

void ThingspeakWebSendTask::clear() {
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        this->params[i].setActive(false);
    }
}

String ThingspeakWebSendTask::getServer() {
    return F("api.thingspeak.com");
}

String ThingspeakWebSendTask::getUrl(String key) {
    String url;
    url.reserve(14 + key.length() + this->size() * 15);
    url += SF("/update?api_key=") + key;
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        UrlParam p = this->getParam(i);
        if (p.isActive()) {
            String buf;
            buf.reserve(15);
            buf += F("&field");
            buf += p.getId();
            buf += F("=");
            buf += p.getValue().substring(0, 5);
            url += buf;
        }
    }
    return url;
}

void ThingspeakWebSendTask::setParam(byte id, byte v) {
    UrlParam param = this->getParam(id);
    param.setValue(id, v);
}

void ThingspeakWebSendTask::setParam(byte id, uint16_t v) {
    UrlParam param = this->getParam(id);
    param.setValue(id, v);
}

void ThingspeakWebSendTask::setParam(byte id, float v) {
    UrlParam param = this->getParam(id);
    param.setValue(id, v);
}

byte ThingspeakWebSendTask::size() {
    byte c = 0;
    for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
        if (this->params[i].isActive()) {
            c++;
        }
    }
    return c;
}
