#include "StdInc.h"

CHudColours::CHudColours()
{
    SetRGBAValue(0, 180, 25, 29, 255);
    SetRGBAValue(1, 54, 104, 44, 255);
    SetRGBAValue(2, 50, 60, 127, 255);
    SetRGBAValue(3, 172, 203, 241, 255);
    SetRGBAValue(4, 100, 156, 204, 255);
    SetRGBAValue(5, 225, 225, 225, 255);
    SetRGBAValue(6, 0, 0, 0, 255);
    SetRGBAValue(7, 144, 98, 16, 255);
    SetRGBAValue(8, 168, 110, 252, 255);
    SetRGBAValue(9, 150, 150, 150, 255);
    SetRGBAValue(10, 104, 15, 17, 255);
    SetRGBAValue(11, 38, 71, 31, 255);
    SetRGBAValue(12, 226, 192, 99, 255);
    SetRGBAValue(13, 74, 90, 107, 255);
    SetRGBAValue(14, 20, 25, 200, 255);
    SetRGBAValue(15, 255, 255, 0, 255);
}

CRGBA CHudColours::GetRGB(uint8_t id, uint8_t alpha)
{
    return CRGBA(colors[id].red, colors[id].green, colors[id].blue, alpha);
}

CRGBA CHudColours::GetRGBA(uint8_t id)
{
    return colors[id];
}

int CHudColours::GetIntColour(uint8_t id)
{
    /* GTA BUG: instead of and (&) they put or (|)
     */
    return ((colors[id].red << 24) | 0xff) + 
           ((colors[id].green << 16) | 0xff) +
           ((colors[id].blue << 8) | 0xff);
}

void CHudColours::SetRGBAValue(uint8_t id, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    colors[id] = CRGBA(red, green, blue, alpha);
}