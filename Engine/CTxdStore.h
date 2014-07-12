#pragma once

class CTexDictionaryEntry
{ // sizeof = 12 
public:
    RwTexDictionary *dictionary; 
    uint16_t usageCount;
    uint16_t parentIndex;
    uint32_t hash;
};

class CTxdStore
{
public:
	static void AddRef(int iIndex); /// DONE
	static void AddTxdSlot(char const *name, char const*, bool); // DONE
	static void Create(int index); // DONE
	static int16_t FindTxdSlot(const char *szName); // DONE
	static int16_t FindTxdSlotFromHashKey(int hash); // DONE
	static bool FinishLoadTxd(int index, RwStream *stream); // DONE
	static void GameShutdown(); // DONE
	static size_t GetNumRefs(int index); // DONE
	static void GetSpecialTxdSlot(int);
	static RwTexDictionary *GetTxdParent(RwTexDictionary *dict); // DONE
	static void Initialise(); // DONE
	static void LoadTxd(int, RwStream*);
	static void LoadTxd(int, char const*);
    // Plugin>
	static RwBool PluginAttach(); // DONE
    static void *PluginConstructor(void *clump, RwInt32 offset, RwInt32 size); // DONE
    static void *PluginDestructor(void *clump, RwInt32 offset, RwInt32 size); // DOONE
    static void *PluginCopyConstructor(void *dstClump, const void *srcClump, RwInt32 offset, RwInt32 size); // DONE
    // <Plugin
	static RwTexDictionary *PopCurrentTxd(); // DONE
	static void PushCurrentTxd(); // DONE
	static void RemoveRef(int index); // DONE
	static void RemoveRefWithoutDelete(int index); // DONE
	static void RemoveTxd(int index); // DONE
	static void RemoveTxdSlot(int index); // DONE
	static void SetCurrentTxd(int index); // DONE
	static void SetTxdParent(RwTexDictionary *dict, RwTexDictionary *parent); // DONE
	static void SetupTxdParent(int index); // DONE
	static void Shutdown(); // DONE
	static bool StartLoadTxd(int index, RwStream *stream); // DONE
	static RwTexture *TxdStoreFindCB(const RwChar *name); // DONE
private:
	static int16_t ms_lastSlotFound;
	static CPool<CTexDictionaryEntry> *ms_pTxdPool;
};