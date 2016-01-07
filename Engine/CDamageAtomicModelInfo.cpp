#include "StdInc.h"

CDamageAtomicModelInfo* CDamageAtomicModelInfo::AsAtomicModelInfoPtr()
{
    return this;
}

void CDamageAtomicModelInfo::SetDamagedAtomic(RpAtomic* atomic)
{
    m_damagedAtomic = atomic;
    if ( CCustomBuildingRenderer::IsCBPCPipelineAttached(atomic))
    {
        CCustomBuildingRenderer::AtomicSetup(atomic);
    }
    else if (CCarFXRenderer::IsCCPCPipelineAttached(atomic))
    {
        CCarFXRenderer::SetCustomFXAtomicRenderPipelinesVMICB(atomic,NULL);
    }
}

void CDamageAtomicModelInfo::Init()
{
    CAtomicModelInfo::Init();
    m_damagedAtomic = NULL;
}

void CDamageAtomicModelInfo::DeleteRwObject()
{
    if (m_damagedAtomic)
    {
        RwFrame* frame = (RwFrame*)m_damagedAtomic->object.object.parent;
        RpAtomicDestroy(m_damagedAtomic);
        RwFrameDestroy(frame);
        m_damagedAtomic = NULL;
    }
    CAtomicModelInfo::DeleteRwObject();
}

void* CDamageAtomicModelInfo::CreateInstance(RwMatrixTag* matrix)
{
    if (m_bCreateDamagedVersion)
    {
        if (m_damagedAtomic)
        {
            RpAtomic* atomic = RpAtomicClone(m_damagedAtomic);
            RwFrame* frame = RwFrameCreate();
            memcpy(&frame->modelling, matrix, sizeof(frame->modelling));
            RpAtomicSetFrame(atomic, frame);
            return (void*)atomic;
        }
    }
    else
    {
        return CAtomicModelInfo::CreateInstance(matrix);
    }
}

void* CDamageAtomicModelInfo::CreateInstance()
{
    if ( CDamageAtomicModelInfo::m_bCreateDamagedVersion )
    {
        if (!m_damagedAtomic)
        {
            return NULL;
        }
        RpAtomic* atomic = RpAtomicClone(m_damagedAtomic);
        RpAtomicSetFrame(atomic, RwFrameCreate());
        return atomic;
    }
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