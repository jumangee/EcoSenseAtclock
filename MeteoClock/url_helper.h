#ifndef _URLWEBSENDHELPER_H
#define _URLWEBSENDHELPER_H

    #include <Arduino.h>
    //#include <WString.h>
    #include "LinkedList/LinkedList.h"
    //#include "processy_cfg.h"
    #include "meteo_messages.h"

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

    class ParamsWebSendTask {
        protected:
            LinkedList<UrlParam*> params = LinkedList<UrlParam*>();

        public:
            ParamsWebSendTask() {
            }

            UrlParam* getParam(byte id);

            void clear();

            ~ParamsWebSendTask();
    };

    /**
     * ThingSpeak API implementation
     */
    class ThingspeakWebSendTask: public ParamsWebSendTask {
        public:
            ThingspeakWebSendTask(): ParamsWebSendTask() {
            }

            String getServer();

            String getUrl(String key);

            void setParam(byte id, byte v);

            void setParam(byte id, uint16_t v);

            void setParam(byte id, float v);

            byte size();
    };

#endif