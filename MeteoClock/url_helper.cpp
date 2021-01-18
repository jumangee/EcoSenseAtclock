#include "url_helper.h"
#include "LinkedList/LinkedList.h"

            //--@include "processy_cfg.h"
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

UrlParam* ParamsWebSendTask::getParam(byte id) {
    for (int i = 0; i < this->params.size(); i++) {
        UrlParam* param = this->params.get(i);
        if (param->getId() == id) {
            return param;
        }
    }
    return NULL;
}

void ParamsWebSendTask::clear() {
    for (int i = 0; i < this->params.size(); i++) {
        this->params.get(i)->setActive(false);
    }
}

ParamsWebSendTask::~ParamsWebSendTask() {
    params.clear();
}

String ThingspeakWebSendTask::getServer() {
    return F("api.thingspeak.com");
}

String ThingspeakWebSendTask::getUrl(String key) {
    String url;
    url.reserve(14 + key.length() + this->params.size() * 15);
    url += SF("/update?api_key=") + key;
    for (int i = 0; i < this->params.size(); i++) {
        UrlParam *p = this->params.get(i);
        if (p->isActive()) {
            String buf;
            buf.reserve(15);
            buf += F("&field");
            buf += p->getId();
            buf += F("=");
            buf += p->getValue().substring(0, 5);
            url += buf;
        }
    }
    return url;
}

void ThingspeakWebSendTask::setParam(byte id, byte v) {
    UrlParam* param = this->getParam(id);
    if (param) {
        param->setValue(id, v);
        return;
    }
    param = new UrlParam();
    param->setValue(id, v);
    this->params.add(param);
}

void ThingspeakWebSendTask::setParam(byte id, uint16_t v) {
    UrlParam* param = this->getParam(id);
    if (param) {
        param->setValue(id, v);
        return;
    }
    param = new UrlParam();
    param->setValue(id, v);
    this->params.add(param);
}

void ThingspeakWebSendTask::setParam(byte id, float v) {
    UrlParam* param = this->getParam(id);
    if (param) {
        param->setValue(id, v);
        return;
    }
    param = new UrlParam();
    param->setValue(id, v);
    this->params.add(param);
}

byte ThingspeakWebSendTask::size() {
    byte c = 0;
    for (int i = 0; i < this->params.size(); i++) {
        if (this->params.get(i)->isActive()) {
            c++;
        }
    }
    return c;
}
