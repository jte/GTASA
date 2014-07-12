#pragma once

class CKeyGen
{
public:
    static uint32_t GetKey(char const *szString);
	static uint32_t GetKey(char const *szString, int uiLength);
	static uint32_t GetUppercaseKey(char const *szString);
	static uint32_t AppendStringToKey(unsigned int uiKey, char const *szString);
private:
    static uint32_t keyTable[256];
};