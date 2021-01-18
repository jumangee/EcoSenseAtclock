#ifndef _STUFF_FIRMWARE_FRAMEWORK_H
#define _STUFF_FIRMWARE_FRAMEWORK_H

#include <Arduino.h>

class rgbColor {
    public:
        float r;
        float g;
        float b;

        rgbColor(float r, float g, float b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        static rgbColor InverseSrgbCompanding(rgbColor c);

        static rgbColor SrgbCompanding(rgbColor c);

        static rgbColor ColorMix(rgbColor color1, rgbColor color2, float mix);
};

#endif