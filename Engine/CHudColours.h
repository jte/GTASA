#pragma once

class CHudColours
{
public:
    CHudColours();
    static CRGBA GetRGB(uint8_t index, uint8_t alpha);
    static CRGBA GetRGBA(uint8_t index);
	static int GetIntColour(uint8_t id);
	static void SetRGBAValue(uint8_t id, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	~CHudColours();
private:
    static CRGBA colors[15];
};