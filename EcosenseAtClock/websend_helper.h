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

            UrlParam();
            
            void set(vType type);

            void setValue(uint16_t v);

            void setValue(float v);

            String getValue();
    };

    /**
     * ThingSpeak API implementation
     */
    class ThingspeakWebSendTask {
            UrlParam                    params[THINGSPEAKPARAMS];
            const __FlashStringHelper*  apiKey;
        public:
            byte        size=0;

            ThingspeakWebSendTask(const __FlashStringHelper* apiKey) {
                this->apiKey = apiKey;
                clear();
            }

            UrlParam& getParam(byte id) {
                return this->params[id];
            }

            void clear();

            const __FlashStringHelper* getApiKey();

            /*//@implement
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
                        buf += p.getValue();
                        url += buf;
                    }
                }
                return url;
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