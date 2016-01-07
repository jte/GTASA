#include "StdInc.h"

size_t CMessages::GetGxtStringLength(unsigned short* string)
{
	size_t len = 0;
	while (string[len])
	{
		len++;
	}
	return len;
}

void CMessages::GxtStringCopy(unsigned short* destination, unsigned short* source, unsigned short length)
{
	size_t i;
	for (i = 0; i < length - 1 && source[i]; i++)
	{
		destination[i] = source[i];
	}
	destination[i] = '\0';
	for (; i < length - 1; i++)
	{
		destination[i] = '\0';
	}
	destination[i] = '\0';
}