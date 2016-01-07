#pragma once

#include "CKeyArray.h"
#include "CData.h"
#include "CMissionTextOffsets.h"

class CText
{
public:
    CText();
	unsigned short* Get(const char* key);
	void GetNameOfLoadedMissionText(char* missionName);
	uint16_t GetUpperCase(uint16_t ch);
	void Load(unsigned char loadMissionPack);
	//* LoadMissionPackText()
	//* LoadMissionText(char*)
	//* ReadChunkHeader(ChunkHeader*, int, unsigned int*, unsigned char)
	//* Unload(unsigned char)
private:
    CKeyArray m_keyMain;
    CData m_dataMain;
    CKeyArray m_keyMission;
    CData m_dataMission;
    uint8_t m_codepage;
    uint8_t m_gotTextOffsets;
    uint8_t m_cdErrorInitialized;
    uint8_t m_missionLoaded;
    char m_missionName[8];
    char m_cdErrorText[256];
    CMissionTextOffsets m_missionTextOffsets;
};

extern CText TheText;