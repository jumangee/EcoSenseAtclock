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

            String getUrl();

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