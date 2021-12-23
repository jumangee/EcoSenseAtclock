#ifndef _URLWEBSENDHELPER_H
#define _URLWEBSENDHELPER_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "ecosense_cfg.h"

    class UrlParam {
            byte paramId;
            union {
                byte b;
                uint16_t ui16;
                float f;
            };
            enum vType {
                BYTE,
                UINT,
                FLOAT
            } valueType;
            
        public:
            //@implement
            UrlParam() {
            }

            //@implement
            byte getId() {
                return this->paramId;
            }
            
            //@implement
            void setId(byte paramId, vType type) {
                this->paramId = paramId;
                this->valueType = type;
            }

            //@implement
            void setValue(byte paramId, byte v) {
                this->b = v;
                setId(paramId, BYTE);
            }

            //@implement
            void setValue(byte paramId, uint16_t v) {
                this->ui16 = v;
                setId(paramId, UINT);
            }

            //@implement
            void setValue(byte paramId, float v) {
                this->f = v;
                setId(paramId, FLOAT);
            }

            //@implement
            String getValue() {
                switch (this->valueType)
                {
                    case BYTE: return String(b);
                    case UINT: return String(ui16);
                    case FLOAT: return String(f);
                }
                return String();
            }
    };

    #define THINGSPEAKPARAMS 8

    /**
     * ThingSpeak API implementation
     
    class ThingspeakWebSendTask {
            UrlParam params[THINGSPEAKPARAMS];
        public:
            ThingspeakWebSendTask() {
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    UrlParam p = params[i];
                    p.setValue(i, byte(0));
                    p.setActive(false);
                }
            }

            UrlParam& getParam(byte id) {
                return this->params[id];
            }

            !//@implement
            void clear() {
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    this->params[i].setActive(false);
                }
            }

            !//@implement
            String getServer() {
                return F("api.thingspeak.com");
            }

            !//@implement
            String getUrl(String key) {
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

            !//@implement
            void setParam(byte id, byte v) {
                UrlParam param = this->getParam(id);
                param.setValue(id, v);
            }

            !//@implement
            void setParam(byte id, uint16_t v) {
                UrlParam param = this->getParam(id);
                param.setValue(id, v);
            }

            !//@implement
            void setParam(byte id, float v) {
                UrlParam param = this->getParam(id);
                param.setValue(id, v);
            }

            !//@implement
            byte size() {
                byte c = 0;
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    if (this->params[i].isActive()) {
                        c++;
                    }
                }
                return c;
            }
    };*/

#endif