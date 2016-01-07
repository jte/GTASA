#pragma once

class FxInterpInfoU255_c
{
public:
    FxInterpInfoU255_c();
	void GetVal(float* values, float time);
	void Load(int file);
	~FxInterpInfoU255_c();
private:
    uint16_t** m_valueArray;
};