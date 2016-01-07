#include "StdInc.h"

/* virtual methods */

void CBaseModelInfo::SetAnimFile(char const *szName)
{
    m_textureDictionary = CTxdStore::FindTxdSlot(szName);
    if (m_textureDictionary != -1)
    {
        m_textureDictionary = CTxdStore::AddTxdSlot(szName);
    }
}

size_t CBaseModelInfo::GetAnimFileIndex()
{
    return -1;
}

/* << */

CBaseModelInfo::CBaseModelInfo()
{
    m_refCount = 0;
    m_textureDictionary = -1;
}



void CBaseModelInfo::ClearTexDictionary()
{
    m_textureDictionary = -1;
}

void CBaseModelInfo::AddTexDictionaryRef()
{
    CTxdStore::AddRef(m_textureDictionary);
}

void CBaseModelInfo::RemoveTexDictionaryRef()
{
    CTxdStore::RemoveRef(m_textureDictionary);
}

void CBaseModelInfo::AddRef()
{
    m_refCount++;
    AddTexDictionaryRef();
}

void CBaseModelInfo::RemoveRef()
{
    m_refCount--;
    RemoveTexDictionaryRef();
}

void CBaseModelInfo::Init()
{
    m_refCount = 0;
    m_textureDictionary = -1;
    m_colModel = NULL;
    m_2dEffectIndex = -1;
    m_2dEffectCount = 0;
    m_objectInfoIndex = -1;
    m_loadDistanceUnscaled = 2000.0f;
    m_rwObject = 0;
    m_doWeOwnTheColModel = true;//128
    m_drawnLast = true; //64
}

void CBaseModelInfo::DeleteCollisionModel()
{
    if (m_colModel && m_doWeOwnTheColModel)
    {
        delete m_colModel;
    }
    m_colModel = NULL;
}

void CBaseModelInfo::SetColModel(CColModel *colModel, bool pairOtherModel)
{
    m_colModel = colModel;
    if (!pairOtherModel)
    {
        m_doWeOwnTheColModel = false;
    }
    else
    {
        m_doWeOwnTheColModel = true;
        CTimeInfo *timeInfo = GetTimeInfo();
        if (timeInfo)
        {
            if (timeInfo->m_objectId != -1)
            {
                CBaseModelInfo *info = CModelInfo::GetModelInfo(timeInfo->m_objectId);
                info->m_colModel = colModel;
                info->m_doWeOwnTheColModel = false;
            }
        }
    }
}

Plugin2dEffectDescriptor* CBaseModelInfo::Get2dEffect(size_t id)
{
    Plugin2dEffect* fx = NULL;
    RpAtomic* atomic = NULL;
    switch (GetRwModelType())
    {
        case RW_TYPE_ATOMIC:
            atomic = reinterpret_cast<RpAtomic*>(m_rwObject);
        break;
        case RW_TYPE_CLUMP:
        {
            RpClump* clump = reinterpret_cast<RpClump*>(m_rwObject);
            if (clump)
            {
                RpAtomic* atomic = Get2DEffectAtomic(clump);
            }
        }
        break;
    }
    if (atomic)
    {
        fx = PLUGIN_2DEFFECT(atomic->geometry, data);
        if ( fx )
        {
            m_2dEffectCount -= fx->count;
        }
    }
    if (id < m_2dEffectCount)
        return &CModelInfo::Get2dEffectStore()[16 * (id + m_2dEffectIndex) + 1];
    else
    {
        return &fx->entries[id - m_2dEffectCount];
    }
}

void CBaseModelInfo::Init2dEffects()
{
    m_2dEffectCount = 0;
    m_2dEffectIndex = -1;
}

void CBaseModelInfo::SetTexDictionary(const char* txdName)
{
    m_textureDictionary =  CTxdStore::FindTxdSlot(txdName);
    if (m_textureDictionary == -1)
    {
        m_textureDictionary = CTxdStore::AddTxdSlot(txdName);
    }
}

void CBaseModelInfo::Add2dEffect(C2dEffect* a2)
{
    if (m_2dEffectIndex == -1)
    {
        m_2dEffectCount = 1;
        m_2dEffectIndex = (a2 - (_DWORD)CModelInfo::Get2dEffectStore() - 4) / 64;
    }
    else
    {
        m_2dEffectCount++;
    }
}

void CBaseModelInfo::Shutdown()
{
    DeleteRwObject();
    if (m_colModel && m_doWeOwnTheColModel)
    {
        delete m_colModel;
    }
    m_doWeOwnTheColModel = true; /// supposed to be set to false ??
    m_colModel = NULL;
    m_2dEffectIndex = -1;
    m_2dEffectCount = 0;
    m_objectInfoIndex = -1;
    m_textureDictionary = -1;
}

bool CBaseModelInfo::IsLod() const
{
    return m_isLod;
}

bool CBaseModelInfo::GetDontCastShadowsOn() const
{
    return m_dontCastShadowsOn;
}

bool CBaseModelInfo::GetHasAnimation() const
{
	return m_hasAnimation;
}

bool CBaseModelInfo::GetHasBeenPrerendered() const
{
	return bHasBeenPreRendered;
}

void CBaseModelInfo::SetHasBeenPrerendered(bool hasBeenPrerendered)
{
	bHasBeenPreRendered = hasBeenPrerendered;
}

void CBaseModelInfo::SetDrawAdditive(bool drawAdditive)
{
    bDrawAdditive = drawAdditive;
}

bool CBaseModelInfo::GetDoWeOwnColModel() const
{
	return m_doWeOwnTheColModel;
}