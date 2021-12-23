#ifndef _URLWEBSENDHELPER_H
#define _URLWEBSENDHELPER_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "ecosense_cfg.h"

    class UrlParam {
            //byte paramId;
            union {
                uint16_t ui16;
                float f;
            };
            enum vType {
                UINT,
                FLOAT
            } valueType;
            
        public:
            bool active;

            //@implement
            UrlParam() {
                this->active = false;
            }

            /*//@implement
            byte getId() {
                return this->paramId;
            }*/
            
            //@implement
            void set(/*byte paramId,*/ vType type) {
                //this->paramId = paramId;
                this->valueType = type;
                this->active = true;
            }

            //@implement
            void setValue(/*byte paramId,*/ uint16_t v) {
                this->ui16 = v;
                set(UINT);
            }

            //@implement
            void setValue(/*byte paramId,*/ float v) {
                this->f = v;
                set(FLOAT);
            }

            //@implement
            String getValue() {
                switch (this->valueType)
                {
                    //case BYTE: return String(b);
                    case UINT: return String(ui16);
                    case FLOAT: return String(f);
                }
                return String();
            }

            //@implement
            bool isActive() {
                return this->active;
            }
    };

    /**
     * ThingSpeak API implementation
     */
    class ThingspeakWebSendTask {
            UrlParam    params[THINGSPEAKPARAMS];
        public:
            byte        size=0;

            ThingspeakWebSendTask() {
                clear();
            }

            UrlParam& getParam(byte id) {
                return this->params[id];
            }

            //@implement
            void clear() {
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    this->params[i].setValue((uint16_t)0);
                    this->params[i].active = false;
                }
                size = 0;
            }

            //@implement
            String getUrl(String key) {
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

            /*//@implement
            void setParam(byte id, byte v) {
                UrlParam param = this->getParam(id);
                param.setValue(id, v);
                size++;
            }

            !//@implement
            void setParam(byte id, uint16_t v) {
                UrlParam param = this->getParam(id);
                param.setValue(id, v);
                size++;
            }

            !//@implement
            void setParam(byte id, float v) {
                UrlParam param = this->getParam(id);
                param.setValue(id, v);
                size++;
            }*/

            //!@implement
            /*byte size() {
                byte c = 0;
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    if (this->params[i].isActive()) {
                        c++;
                    }
                }
                return c;
            }*/

            void recount() {
                byte c = 0;
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    if (this->params[i].active) {
                        c++;
                    }
                }
                this->size=c;
            }
    };

#endif