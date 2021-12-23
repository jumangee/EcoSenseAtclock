#ifndef _URLWEBSENDHELPER_H
#define _URLWEBSENDHELPER_H

    #include <Arduino.h>
    #include "processy_cfg.h"
    #include "ecosense_cfg.h"

    class UrlParam {
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
            
            //@implement
            void set(vType type) {
                this->valueType = type;
                this->active = true;
            }

            //@implement
            void setValue(uint16_t v) {
                this->ui16 = v;
                set(UINT);
            }

            //@implement
            void setValue(float v) {
                this->f = v;
                set(FLOAT);
            }

            //@implement
            String getValue() {
                switch (this->valueType)
                {
                    //case BYTE: return String(b);
                    case UINT: return String(ui16);
                }
                return String(f);
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
                    //this->params[i].setValue((uint16_t)0);
                    this->params[i].active = false;
                }
                size = 0;
            }

            //@implement
            String getUrl() {
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