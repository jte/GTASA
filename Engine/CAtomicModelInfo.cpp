#include "StdInc.h"

CAtomicModelInfo* CAtomicModelInfo::AsAtomicModelInfoPtr()
{
    return this;
}

void CAtomicModelInfo::SetAtomic(RpAtomic* atomic)
{
    RpAtomic* self = reinterpret_cast<RpAtomic*>(m_rwObject);
    if (m_rwObject)
    {
        Plugin2dEffect* effect = PLUGIN_2DEFFECT(self->geometry, data);
        m_2dEffectCount -= effect ? effect->count : 0;
    }
    m_rwObject = reinterpret_cast<RwObject*>(atomic);
    Plugin2dEffect* effect = PLUGIN_2DEFFECT(atomic->geometry, data);
    m_2dEffectCount += effect ? effect->count : 0;
    AddTexDictionaryRef();
    if (GetAnimFileIndex() != -1)
    {
        CAnimManager::AddAnimBlockRef(GetAnimFileIndex());
    }
    if (CCustomBuildingRenderer::IsCBPCPipelineAttached(atomic))
    {
        CCustomBuildingRenderer::AtomicSetup(atomic);
    }
    else if (CCarFXRenderer::IsCCPCPipelineAttached(atomic))
    {
        CCarFXRenderer::SetCustomFXAtomicRenderPipelinesVMICB(atomic, NULL);
    }
    if (m_doWeOwnTheColModel && bDontCollideWithFlyer && bCollisionWasStreamedWithModel)
    {
        CTagManager::RestAlpha(atomic);
    }
    bHasBeenPreRendered = true; //m_flags |= 1;
}

void CAtomicModelInfo::DeleteRwObject()
{
    RpAtomic* atomic = reinterpret_cast<RpAtomic*>(m_rwObject);
    if (atomic)
    {
        Plugin2dEffect* fx = PLUGIN_2DEFFECT(atomic, data);
        m_2dEffectCount -= fx ? fx->count : 0;
        RwFrame* frame = RpAtomicGetFrame(atomic);
        RpAtomicDestroy(atomic);
        RwFrameDestroy(frame);
        m_rwObject = NULL;
        RemoveTexDictionaryRef();
        if (GetAnimFileIndex() != -1)
        {
            CAnimManager::RemoveAnimBlockRef(GetAnimFileIndex());
        }
    }
}

void* CAtomicModelInfo::CreateInstance(RwMatrixTag* matrix)
{
    if (!m_rwObject)
    {
        return NULL;
    }
    AddRef();
    RpAtomic* atomic = RpAtomicClone((RpAtomic *)m_rwObject);
    RwFrame* frame = RwFrameCreate();
    memcpy(&frame->modelling, matrix, sizeof(frame->modelling));
    RpAtomicSetFrame(atomic, frame);
    RemoveRef();
    return (void*)atomic;
}

void* CAtomicModelInfo::CreateInstance()
{
    if (!m_rwObject)
    {
        return NULL;
    }
    AddRef();
    RpAtomic* atomic = RpAtomicClone((RpAtomic*)m_rwObject);
    RpAtomicSetFrame(atomic, RwFrameCreate());
    RemoveRef();
    return atomic;
}

size_t CAtomicModelInfo::GetModelType()
{
    return 1;
}

size_t CAtomicModelInfo::GetRwModelType()
{
    return RW_TYPE_ATOMIC;
}

void CAtomicModelInfo::Init()
{
	CBaseModelInfo::Init();
}

RpAtomic* CAtomicModelInfo::GetAtomicFromDistance(float distance)
{
	if (TheCamera.GenerationDistMultiplier * this->m_loadDistanceUnscaled <= distance)
	{
		return NULL;
	}
	else
	{
		return (RpAtomic*)GetRwObject();
	}
}