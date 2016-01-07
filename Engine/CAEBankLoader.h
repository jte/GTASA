#pragma once

struct CAEBankSlotItem
{
    int m_offset;
    int m_loopOffset;
    short m_sampleRate;
    short m_soundHeadroom;
};

struct CAEBankLookupItem
{
    char m_pak;
    char _pad[3];
    int m_offset;
    int m_size;
};

struct CAEBankSlot
{
    int m_offsetOnBuffer;
    int m_slotBufferSize;
    int _unknown1;
    int _unknown2;
    unsigned short m_bankNum;
    unsigned short m_soundsOnBank;
    CAEBankSlotItem m_bankItems[400];
};

struct CAESoundRequest
{
    CAEBankSlot* m_bankSlot;
    int m_offset;
    int m_size;
    void* m_bufferData;     // Somewhere in the m_pBuffer, used to resolve sector alignment issues
    void* m_buffer;         // Pointer returned by malloc
    int m_loadingStatus;
    short m_bank;
    short m_bankSlot;
    short m_sound;
    char m_pak;
    char field_1F;
};

class CAEBankLoader
{
public:
	CAEBankLoader();
    bool Initialise();
	/* Initialise()
	* LoadSFXPakLookupFile()
	*/ 
    void* GetSoundBuffer(uint16_t bankSlotId, int* slotSize);
    bool LoadBankSlotFile();
    bool LoadBankLookupFile();
    void InitialiseRequestList();
    void CalculateBufferSize();
    ~CAEBankLoader();
private:
    CAEBankSlot* m_bankSlots;
    CAEBankLookupItem* m_bankLookup;
    void* m_pakFiles;
    short m_numBankSlots;
    short m_numBanks;
    short m_numPakFiles;
    short _unused0;
    char m_initialized;
    char gap_15[3];
    int m_soundBuffersSize;
    char* m_soundBuffers;
    int* m_streamHandles;
    CAESoundRequest m_soundRequests[50];
    short pad1;
    short m_requestsToLoad;
    short m_requestListNext;
    short m_streamingChannel;
    short m_bankSlotSound[45];
    char _unused1[30];
};