#include "stuff.h"

static rgbColor rgbColor::InverseSrgbCompanding(rgbColor c) 
{
    //Convert color from 0..255 to 0..1
    float r = c.r / 255;
    float g = c.g / 255;
    float b = c.b / 255;
    //Inverse Red, Green, and Blue
    if (r > 0.04045) r = pow((r+0.055)/1.055, 2.4); else r = r / 12.92;
    if (g > 0.04045) g = pow((g+0.055)/1.055, 2.4); else g = g / 12.92;
    if (b > 0.04045) b = pow((b+0.055)/1.055, 2.4); else b = b / 12.92;
    //return new color. Convert 0..1 back into 0..255
    return rgbColor(r*255, g*255, b*255);
}

static rgbColor rgbColor::SrgbCompanding(rgbColor c) 
{
    //Convert color from 0..255 to 0..1
    float r = c.r / 255;
    float g = c.g / 255;
    float b = c.b / 255;
    //Apply companding to Red, Green, and Blue
    if (r > 0.0031308) r = 1.055*pow(r, 1/2.4)-0.055; else r = r * 12.92;
    if (g > 0.0031308) g = 1.055*pow(g, 1/2.4)-0.055; else g = g * 12.92;
    if (b > 0.0031308) b = 1.055*pow(b, 1/2.4)-0.055; else b = b * 12.92;
    //return new color. Convert 0..1 back into 0..255
    return rgbColor(r*255, g*255, b*255);
}

static rgbColor rgbColor::ColorMix(rgbColor color1, rgbColor color2, float mix) 
{
    //Mix [0..1]
    //  0   --> all c1
    //  0.5 --> equal mix of c1 and c2
    //  1   --> all c2
    //Invert sRGB gamma compression
    rgbColor c1 = rgbColor::InverseSrgbCompanding(color1);
    rgbColor c2 = rgbColor::InverseSrgbCompanding(color2);
    //Reapply sRGB gamma compression
    return rgbColor::SrgbCompanding(rgbColor(
                    c1.r*(1-mix) + c2.r*(mix),
                    c1.g*(1-mix) + c2.g*(mix),
                    c1.b*(1-mix) + c2.b*(mix)
                ));
}
