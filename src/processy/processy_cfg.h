#ifndef _PROCESSY_CFG_H
#define _PROCESSY_CFG_H

    /*** processy config ***/
    #define DEBUG_PRO_MS
    #define DEBUG_PRO_PERIOD_MS 30000
    #define DEBUG_SERIAL

    /*** processy inlines ***/
    #define S(t) String(t)
    #define SF(t) String(F(t))
    #define TRACELN(t) Serial.println(t);
    #define TRACELNF(t) Serial.println(String(F(t)));
    #define TRACE(t) Serial.print(t);
    #define TRACEF(t) Serial.print(String(F(t)));

#endif