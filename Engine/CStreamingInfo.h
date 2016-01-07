#pragma once

class CStreamingInfo
{
public:
    void Init();
	void AddToList(CStreamingInfo* info);
    void RemoveFromList();
    CStreamingInfo* GetNext();
    CStreamingInfo* GetPrev();
    bool InList();
    uint32_t GetCdSize();
	/* GetCdPosn()
	* GetCdPosnAndSize(unsigned int&, unsigned int&)
	* SetCdPosnAndSize(unsigned int, unsigned int)*/
public:
    static CStreamingInfo *ms_pArrayBase;
    uint16_t m_nextIndex;
    uint16_t m_prevIndex;
    uint16_t pad1;
    uint8_t m_flags;
    uint8_t m_fileOffset;
    uint32_t pad2;
    uint32_t m_fileSize;
    uint8_t m_loadStatus; // 0, 1 = loaded, 2, 3, 4 = loading large file
    uint8_t pad3[3];
};