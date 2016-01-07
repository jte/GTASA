#pragma once

class CRGBA
{
public:
    CRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	CRGBA()
	{
		red = 0;
		green = 0;
		blue = 0;
		alpha = 0;
	}
    operator RwUInt32() const
    {
        return D3DCOLOR_RGBA(red, green, blue, alpha);
    }
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};