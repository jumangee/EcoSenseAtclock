#ifndef _URLWEBSENDHELPER_H
#define _URLWEBSENDHELPER_H

    #include <Arduino.h>
    //#include <WString.h>
    //#include "LinkedList/LinkedList.h"
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
            bool active;
            
        public:
            UrlParam();

            byte getId();
            
            void setId(byte paramId, vType type);

            void setValue(byte paramId, byte v);

            void setValue(byte paramId, uint16_t v);

            void setValue(byte paramId, float v);

            String getValue();

            bool isActive();

            void setActive(bool s);
    };

    /*class ParamsWebSendTask {
        protected:
            LinkedList<UrlParam*> params = LinkedList<UrlParam*>();

        public:
            ParamsWebSendTask() {
            }

            //!@implement
            //!@include "LinkedList/LinkedList.h"
            UrlParam* getParam(byte id) {
                for (int i = 0; i < this->params.size(); i++) {
                    UrlParam* param = this->params.get(i);
                    if (param->getId() == id) {
                        return param;
                    }
                }
                return NULL;
            }

            //!@implement
            void clear() {
                for (int i = 0; i < this->params.size(); i++) {
                    this->params.get(i)->setActive(false);
                }
            }

            //!@implement
            ~ParamsWebSendTask() {
                params.clear();
            }
    };*/

    #define THINGSPEAKPARAMS 8

    /**
     * ThingSpeak API implementation
     */
    class ThingspeakWebSendTask {
            UrlParam params[7]; // 0-8
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

            void clear();

            String getServer();

            String getUrl(String key);

            void setParam(byte id, byte v);

            void setParam(byte id, uint16_t v);

            void setParam(byte id, float v);

            byte size();
    };

#endif