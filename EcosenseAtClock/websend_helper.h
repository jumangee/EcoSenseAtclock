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

            UrlParam();

            /*//@implement
            byte getId() {
                return this->paramId;
            }*/
            
            void set(/*byte paramId,*/ vType type);

            void setValue(/*byte paramId,*/ uint16_t v);

            void setValue(/*byte paramId,*/ float v);

            String getValue();

            bool isActive();
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

            void clear();

            String getUrl(String key);

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