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
            //@implement
            void set(vType type) {
                this->valueType = type;
                //this->active = true;
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
            UrlParam*                   params[THINGSPEAKPARAMS];
            const __FlashStringHelper*  apiKey;
        public:
            byte        size=0;

            ThingspeakWebSendTask(const __FlashStringHelper* apiKey) {
                this->apiKey = apiKey;
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    this->params[i] = NULL;
                }
            }

            UrlParam* getParam(byte id) {
                if (!isParam(id)) {
                    this->params[id] = new UrlParam();
                }
                return this->params[id];
            }

            //@implement
            void clear() {
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    if (isParam(i)) {
                        delete this->params[i];
                    }
                    this->params[i] = NULL;
                }
                size = 0;
            }

            //@implement
            const __FlashStringHelper* getApiKey() {
                return apiKey;
            }

            bool isParam(byte id) {
                return (this->params[id] != NULL);
            }

            void recount() {
                byte c = 0;
                for (byte i = 0; i < THINGSPEAKPARAMS; i++) {
                    if (isParam(i)) {
                        c++;
                    }
                }
                this->size=c;
            }
    };

#endif