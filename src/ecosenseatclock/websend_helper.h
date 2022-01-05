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
                return (this->valueType == UINT) ? String(ui16) : String(f);
            }
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

            //@implement
            void clear() {
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    this->params[i].active = false;
                }
                size = 0;
            }

            //@implement
            const __FlashStringHelper* getApiKey() {
                return apiKey;
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