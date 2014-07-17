#include "StdInc.h"

CAtomicModelInfo* CAtomicModelInfo::AsAtomicModelInfoPtr()
{
    return this;
}

void CAtomicModelInfo::SetAtomic(RpAtomic* atomic)
{
    if(m_rwObject)
    {
        Plugin2dEffect* effect = PLUGIN_2DEFFECT(m_rwObject->geometry);
        m_2dEffectCount -= effect ? effect->count : 0;
    }
    m_rwObject = atomic;
    RpGeometry* geometry = atomic->geometry;
    Plugin2dEffect* effect = PLUGIN_2DEFFECT(geometry);
    m_2dEffectCount += effect ? effect->count : 0;
    AddTexDictionaryRef();
    if(GetAnimFileIndex() != -1)
    {
        CAnimManager::AddAnimBlockRef(GetAnimFileIndex());
    }
    if(CCustomBuildingRenderer::IsCBPCPipelineAttached(atomic))
    {
        CCustomBuildingRenderer::AtomicSetup(atomic);
    }
    else if(CCarFXRenderer::IsCCPCPipelineAttached(atomic))
    {
        CCarFXRenderer::SetCustomFXAtomicRenderPipelinesVMICB(atomic, NULL);
    }
    if((m_flags & 0x80) && (m_flags & 0x7800) && (m_flags & 0x3000))
    {
        CTagManager::RestAlpha(atomic);
    }
    m_flags |= 1;
}