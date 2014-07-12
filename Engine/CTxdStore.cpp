#include "StdInc.h"

#define rwID_TXDSTORE (0xF5)

#define PLUGIN_TXDSTORE(clump, var) \
    (RWPLUGINOFFSET(TxdStorePluginDef, clump, gTxdStorePlugin)->var)

struct TxdStorePluginDef
{
    RwTexDictionary *parent;
};

static RwInt32 gTxdStorePlugin = -1;
static RwTexDictionary *gTxdDictStack = NULL;

int16_t CTxdStore::FindTxdSlot(const char *szName)
{
    size_t hash = CKeyGen::GetUppercaseKey(szName);
    size_t slot;
    // search [0, ms_lastSlotFound]
    for(slot = 0; slot <= ms_lastSlotFound; slot++)
    {
        CTexDictionaryEntry *entry = ms_pTxdPool->Get(slot);
        if(entry->hash == hash)
        {
            break;
        }
    }
    // search <ms_lastSlotFound, ms_pTxdPool->GetSize()>
    if(slot == ms_lastSlotFound)
    {
        slot = ms_lastSlotFound + 1;
        for(slot = ms_lastSlotFound + 1; slot < ms_pTxdPool->GetSize(); slot++)
        {
            CTexDictionaryEntry *entry = ms_pTxdPool->Get(slot);
            if(entry->hash == hash)
            {
                break;
            }
        }
        if(slot >= ms_pTxdPool->m_size)
        {
            return -1;
        }
    }
    ms_lastSlotFound = slot;
    return slot;
}

void CTxdStore::AddRef(int iIndex)
{
    ms_pTxdPool->Get(iIndex)->usageCount++;
}

static void *TxdParentConstructor(void *clump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_TXDSTORE(clump, parent) = 0;
    return clump;
}

void *TxdParentDestructor(void *clump, RwInt32 offset, RwInt32 size)
{
    return clump;
}

void *TxdParentCopyConstructor(void *dstClump, const void *srcClump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_TXDSTORE(dstClump, parent) = PLUGIN_TXDSTORE(srcClump, parent);
    return dstClump;
}


RwBool CTxdStore::PluginAttach()
{
    gTxdStorePlugin = RwTexDictionaryRegisterPlugin(sizeof(TxdStorePluginDef), 
                                                    MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_TXDSTORE),
                                                    TxdParentConstructor,
                                                    TxdParentDestructor,
                                                    TxdParentCopyConstructor);
    return gTxdStorePlugin != -1;
}

RwTexDictionary *CTxdStore::PopCurrentTxd()
{
    RwTexDictionary *sp = RwTexDictionarySetCurrent(gTxdDictStack);
    gTxdDictStack = NULL;
    return sp;
}

void CTxdStore::PushCurrentTxd()
{
    gTxdDictStack = RwTexDictionaryGetCurrent();
}

static RwTexture *TxdStoreLoadCB(const RwChar *name, const RwChar *maskName)
{
    return NULL;
}

void CTxdStore::Initialise()
{
    if(!ms_pTxdPool)
    {
        ms_pTxdPool = new CPool<CTexDictionaryEntry>(5000); // or 416
    }
    for(size_t i = 0; i < ms_pTxdPool->GetSize(); i++)
    {
        AddTxdSlot("*", "player", false);
    }
    RwTextureSetFindCallBack(TxdStoreFindCB);
    RwTextureSetReadCallBack(TxdStoreLoadCB);
}

RwTexture *CTxdStore::TxdStoreFindCB(const RwChar *name)
{
    RwTexDictionary *curr = RwTexDictionaryGetCurrent();
    if(!curr)
    {
        return NULL;
    }
    RwTexDictionary *parent = NULL;
    RwTexture *tex = NULL;
    do
    {
        parent = PLUGIN_TXDSTORE(curr, parent);
        tex = RwTexDictionaryFindNamedTexture(parent, name);
    } while(parent && !tex);
    return tex;
}

void CTxdStore::SetTxdParent(RwTexDictionary *dict, RwTexDictionary *parent)
{
    PLUGIN_TXDSTORE(dict, parent) = parent;
}

RwTexDictionary *CTxdStore::GetTxdParent(RwTexDictionary *dict)
{
    return PLUGIN_TXDSTORE(dict, parent);
}

void CTxdStore::RemoveRef(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry)
    {
        entry->usageCount--;
        if(entry->usageCount <= 0)
        {
            CStreaming::RemoveModel(index + 20000);
        }
    }
}

void CTxdStore::RemoveRefWithoutDelete(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry)
    {
        entry->usageCount--;
    }
}

void CTxdStore::Create(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry)
    {
        entry->dictionary = RwTexDictionaryCreate();
    }
}

void CTxdStore::AddTxdSlot(char const *name, char const*, bool)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->New();
    if(entry)
    {
        entry->dictionary = NULL;
        entry->usageCount = 0;
        entry->parentIndex = -1;
        entry->hash = CKeyGen::GetUppercaseKey(name);
    }
}

int16_t CTxdStore::FindTxdSlotFromHashKey(int hash)
{
    if(ms_pTxdPool->GetSize() == 0)
    {
        return -1;
    }
    for(size_t i = 0; i < ms_pTxdPool->GetSize(); i++)
    {
        CTexDictionaryEntry *entry = ms_pTxdPool->Get(i);
        if(entry && entry->hash == hash)
        {
            return i;
        }
    }
    return -1;
}

static int gTexDictStreamBytesLeft;
static int gTexDictStreamStartPos;

static RwTexture *destroyTexture(RwTexture *texture, void *pData)
{
    RwTextureDestroy(texture);
    return NULL;
}

static size_t texNumLoaded = 0;
static float texLoadTime = 0.0f;

static RwTexture *RwTextureGtaStreamRead(RwStream *stream)
{
    RwUInt32 length;
    RwUInt32 version;
    if(!RwStreamFindChunk(stream, rwID_TEXTURENATIVE, &length, &version))
    {
        return NULL;
    }
    float start_time = CTimer::GetCurrentTimeInCycles() / CTimer::GetCyclesPerMillisecond();
    RwTexture *tex = RwTextureStreamRead(stream); // USE NATIVE TEXTURE READ HERE !! #TODO#
    if(!tex)
    {
        return NULL;
    }
    if(gGameState == 8)
    {
        float dt = CTimer::GetCurrentTimeInCycles() / CTimer::GetCyclesPerMillisecond() - start_time;
        texNumLoaded++;
        texLoadTime = (texNumLoaded * texLoadTime + dt) / texNumLoaded;
    }
    RwUInt8 filter_mode = RwTextureGetFilterModeMacro(tex);
    if(filter_mode == 1)
    {
        RwTextureSetFilterModeMacro(tex, 2);
    }
    else if(filter_mode == 3)
    {
        RwTextureSetFilterModeMacro(tex, 4);
    }
    if(gAnisotropySupported)
    {
        if(RpAnisotTextureGetMaxAnisotropy(tex) >= 1 && g_fx.GetFxQuality() >= 2)
        {
            RpAnisotTextureSetMaxAnisotropy(tex, RpAnisotGetMaxSupportedMaxAnisotropy());
        }
    }
    return tex;
}

static RwTexDictionary *RwTexDictionaryGtaStreamRead1(RwStream *stream)
{
    RwUInt32 length;
    RwUInt32 version;
    gTexDictStreamBytesLeft = 0;
    if(!RwStreamFindChunk(stream, rwID_STRUCT, &length, &version))
    {
        return NULL;
    }
    RwUInt32 buffer;
    if(RwStreamRead(stream, &buffer, length) != length)
    {
        return NULL;
    }
    RwTexDictionary *dict = RwTexDictionaryCreate();
    if(!dict)
    {
        return NULL;
    }
    gTexDictStreamBytesLeft = buffer / 2;
    RwTexture *tex;
    while((tex = RwTextureGtaStreamRead(stream)))
    {
        RwTexDictionaryAddTexture(dict, tex);
        buffer--;
        if(buffer <= gTexDictStreamBytesLeft)
        {
            gTexDictStreamStartPos = stream->position;
            gTexDictStreamBytesLeft = buffer;
            return dict;
        }
    }
    RwTexDictionaryForAllTextures(dict, destroyTexture, NULL);
    RwTexDictionaryDestroy(dict);
    return NULL;
}

bool CTxdStore::StartLoadTxd(int index, RwStream *stream)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(!RwStreamFindChunk(stream, rwID_TEXDICTIONARY, NULL, NULL))
    {
        return false;
    }
    entry->dictionary = RwTexDictionaryGtaStreamRead1(stream);
    return true;
}

static RwTexture *RemoveIfRefCountIsGreaterThanOne(RwTexture *texture, void*)
{
    if(texture->refCount > 1)
    {
        RwTextureDestroy(texture);
        RwTexDictionaryRemoveTexture(texture);
    }
    return texture;
}

void CTxdStore::RemoveTxd(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry)
    {
        RwTexDictionaryForAllTextures(entry->dictionary, RemoveIfRefCountIsGreaterThanOne, NULL);
        RwTexDictionaryDestroy(entry->dictionary);
        RemoveRef(index);
    }
}

void CTxdStore::SetupTxdParent(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry && entry->parentIndex != -1)
    {
        SetTxdParent(entry->dictionary, ms_pTxdPool->Get(entry->parentIndex)->dictionary);
        AddRef(entry->parentIndex);
    }
}

void CTxdStore::SetCurrentTxd(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry)
    {
        RwTexDictionarySetCurrent(entry->dictionary);
    }
}

void CTxdStore::GameShutdown()
{
    for(size_t i = 0; i < ms_pTxdPool->GetSize(); i++)
    {
        CTexDictionaryEntry *entry = ms_pTxdPool->Get(i);
        if(entry && entry->usageCount == 0)
        {
            RemoveTxdSlot(i);
        }
    }
}

void CTxdStore::RemoveTxdSlot(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(entry)
    {
        RwTexDictionaryDestroy(entry->dictionary);
        ms_pTxdPool->Delete(entry);
    }
}

void CTxdStore::Shutdown()
{
    if(ms_pTxdPool)
    {
        delete ms_pTxdPool;
    }
}

size_t CTxdStore::GetNumRefs(int index)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    return entry ? entry->usageCount : 0;
}

static RwTexDictionary *RwTexDictionaryGtaStreamRead2(RwStream *stream, RwTexDictionary *dict)
{
  char *v2; // eax@2
  RwTexDictionary *result; // eax@4

    RwStreamSkip(stream, gTexDictStreamStartPos - stream->position);
    if(gTexDictStreamBytesLeft == 0)
    {
        return dict;
    }
    RwTexture *tex = NULL;
    while((tex = RwTextureGtaStreamRead(stream)))
    {
        gTexDictStreamBytesLeft--;
        RwTexDictionaryAddTexture(dict, tex);
        if(!gTexDictStreamBytesLeft)
        {
            return dict;
        }
    }
    RwTexDictionaryForAllTextures(dict, destroyTexture, 0);
    RwTexDictionaryDestroy(dict);
    return NULL;
}

bool CTxdStore::FinishLoadTxd(int index, RwStream *stream)
{
    CTexDictionaryEntry *entry = ms_pTxdPool->Get(index);
    if(!entry)
    {
        return false;
    }
    entry->dictionary = RwTexDictionaryGtaStreamRead2(stream, entry->dictionary);
    if(!entry->dictionary)
    {
        return false;
    }
    SetupTxdParent(index);
    return true;
}
