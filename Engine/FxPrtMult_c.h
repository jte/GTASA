#pragma once

class FxPrtMult_c
{
public:
    FxPrtMult_c();
	FxPrtMult_c(float, float, float, float, float, float, float);
	void SetUp(float r, float g, float b, float a, float size, float a7, float durationFactor);
private:
    RwRGBAReal m_colour;
    float m_size;
    float pad1;
    float m_durationFactor;
};