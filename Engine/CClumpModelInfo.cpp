#include "StdInc.h"

void CClumpModelInfo::Init()
{
    CBaseModelInfo::Init();
    m_animId = -1;
}

void CClumpModelInfo::Shutdown()
{
    CBaseModelInfo::Shutdown();
}

static RpAtomic *SetAtomicHAnimHierarchyCB(RpAtomic* atomic, void* data)
{
    if (data)
    {
        RpSkinAtomicSetHAnimHierarchy(atomic, (RpHAnimHierarchy*)data);
        return NULL;
    }
    else
    {
        RpSkinAtomicSetHAnimHierarchy(atomic, GetAnimHierarchyFromFrame((RwFrame*)atomic->object.object.parent));
        return atomic;
    }
}

void* CClumpModelInfo::CreateInstance()
{
    if (!m_rwObject)
    {
        return NULL;
    }
    AddRef();
    RpClump* clump = RpClumpClone((RpClump*)m_rwObject);
    RpAtomic* atomic = GetFirstAtomic(clump);
    if (atomic && RpSkinGeometryGetSkin(atomic->geometry) && !dwUnknownFlag26)
    {
        RpHAnimHierarchy *hierAnim = GetAnimHierarchyFromClump(clump);
        RpClumpForAllAtomics(clump, SetAtomicHAnimHierarchyCB, hierAnim);
        RtAnimAnimation *rtAnim = RpAnimBlendCreateAnimationForHierarchy(hierAnim);
        RtAnimInterpolatorSetCurrentAnim(hierAnim->currentAnim, rtAnim);
        hierAnim->flags = rpHANIMHIERARCHYUPDATEMODELLINGMATRICES | rpHANIMHIERARCHYUPDATELTMS;
    }
    if (m_hasAnimation)
    {
        RpAnimBlendClumpInit(clump);
        CAnimBlendHierarchy *anim = CAnimManager::GetAnimation(GetHashKey(), &CAnimManager::AnimBlocks[m_AnimId]);
        if (anim)
        {
            CAnimManager::BlendAnimation(clump, anim, 2, 1.0f);
        }
    }
    RemoveRef();
    return clump;
}

void* CClumpModelInfo::CreateInstance(RwMatrixTag* matrix)
{
    if (!m_rwObject)
    {
        return NULL;
    }
    RpClump* clump = (RpClump*)CreateInstance();
    RwFrame* frame = (RwFrame*)clump->object.parent;
    frame->modelling = *matrix;
    return (void*)clump;
}

void CClumpModelInfo::SetAnimFile(char const* animFile)
{
    if (stricmp(animFile, "null") != 0)
    {
        m_animFile = new char[strlen(animFile) + 1];
        strcpy(m_animFile, animFile);
    }
}

void CClumpModelInfo::ConvertAnimFileIndex()
{
    if (m_animId != -1)
    {
        AnimationId animId = CAnimManager::GetAnimationBlockIndex(m_animFile);
        delete[] m_animFile;
        m_animId = animId;
    }
}

void CClumpModelInfo::DeleteRwObject()
{
    RpClump* clump = (RpClump*)m_rwObject;
    if (clump)
    {
        RpAtomic* atomic = Get2DEffectAtomic(clump);
        if (atomic)
        {
            Plugin2dEffect *data = PLUGIN_2DEFFECT(atomic->geometry, data);
            m_2dEffectCount -= data ? data->count : 0;
        }
        RpClumpDestroy(clump);
        m_rwObject = 0;
        RemoveTexDictionaryRef();
        size_t animIndex = GetAnimFileIndex();
        if (animIndex != -1)
        {
            CAnimManager::RemoveAnimBlockRef(animIndex);
        }
        if (m_doWeOwnTheColModel)
        {
            DeleteCollisionModel();
        }
    }
}

static RpAtomic* AtomicSetupPipelineCB(RpAtomic *atomic, void*)
{
    if (CCustomBuildingRenderer::IsCBPCPipelineAttached(atomic) )
    {
        CCustomBuildingRenderer::AtomicSetup(atomic);
    }
    else if (CCarFXRenderer::IsCCPCPipelineAttached(atomic))
    {
        CCarFXRenderer::CustomCarPipeAtomicSetup(atomic);
    }
    return atomic;
}

void* CClumpModelInfo::SetClump(RpClump *clump)
{
    if (m_rwObject)
    {
        RpAtomic* atomic = Get2DEffectAtomic((RpClump*)m_rwObject);
        if (atomic)
        {
            Plugin2dEffect* fx = PLUGIN_2DEFFECT(atomic->geometry, data);
            m_2dEffectCount -= fx ? fx->count : 0;
        }
    }
    m_rwObject = (RwObject*)clump;
    if (clump)
    {
        RpAtomic* atomic = Get2DEffectAtomic(clump);
        if (atomic)
        {
            Plugin2dEffect* fx = PLUGIN_2DEFFECT(atomic->geometry, data);
            m_2dEffectCount += fx ? fx->count : 0;
        }
    }
    CVisibilityPlugins::SetClumpModelInfo(clump);
    AddTexDictionaryRef();
    if (GetAnimFileIndex() != -1)
    {
        CAnimManager::AddAnimBlockRef(GetAnimFileIndex());
    }
    RpClumpForAllAtomics(clump, AtomicSetupPipelineCB, this);
    RpAtomic* v11 = GetFirstAtomic(clump);
    if (!v11)
    {
        return;
    }
    if (RpSkinGeometryGetSkin(v11->geometry))
    {
        if (dwUnknownFlag26)
        {
            RpClumpForAllAtomics(clump, SetAtomicHAnimHierarchyCB, 0);
        }
        else
        {
            v16 = GetFirstAtomic(clump);
            v16->geometry->morphTarget->boundingSphere.radius *= 1.2;
            v17 = GetAnimHierarchyFromClump(clump);
            v12 = v17;
            v25 = v17;
            RpClumpForAllAtomics(clump, SetAtomicHAnimHierarchyCB, (void *)v17);
            v18 = GetFirstAtomic(clump);
            v14 = v18->geometry;
            v13 = RpSkinGeometryGetSkin(v18->geometry);
            v19 = 0;
            for (v15 = 0; v15 < v14->numVertices; v15++)
            {
                v20 = (RwMatrixWeights *)RpSkinGetVertexBoneWeights(v13);
                v21 = *(float *)((char *)&v20->field_4 + v19) + *(float *)((char *)&v20->field_0 + v19);
                v22 = (float *)((char *)&v20->field_0 + v19);
                v19 += 16;
                v23 = 1.0 / (v21 + v22[2] + v22[3]);
                v22[0] *= v23;
                v22[1] *= v23;
                v22[2] *= v23;
                v22[3] *= v23;
            }
            v17->flags = 0x3000u;
        }
    }
}

RwFrame* CClumpModelInfo::FindFrameFromNameWithoutIdCB(RwFrame* frame, void* data)
{
    int *param = (int*)data;

    if (CVisibilityPlugins::GetFrameHierarchyId(frame) || (stricmp((const char *)param[0], GetFrameNodeName(frame)) != 0))
    {
        RwFrameForAllChildren(frame, FindFrameFromNameWithoutIdCB, param);
        return param[1] ? 0 : frame;
    }
    else
    {
        param[1] = (int)frame;
        return NULL;
    }
}

void CClumpModelInfo::SetFrameIds(RwObjectNameIdAssocation *assoc)
{
    if (!assoc->name)
    {
        return;
    }
    for (; assoc[1].name; assoc++)
    {
        if (!(assoc->flags & 1))
        {
            RwFrame* frame = (RwFrame*)m_rwObject->parent;
            void* param[] = {assoc->name, NULL};
            RwFrameForAllChildren(frame, FindFrameFromNameWithoutIdCB, param);
            if (param[1])
            {
                CVisibilityPlugins::SetFrameHierarchyId((RwFrame*)param[1], assoc->hierarchyID);
            }
        }
    }
}

RwFrame* CClumpModelInfo::FindFrameFromIdCB(RwFrame *frame, void *data)
{
    int* param = (int*)data;
    if (param[0] == CVisibilityPlugins::GetFrameHierarchyId(frame))
    {
        param[1] = (int)frame;
        return NULL;
    }
    else
    {    
        RwFrameForAllChildren(frame, FindFrameFromIdCB, data);
        return param[1] ? NULL : frame;
    }
}

RwFrame* CClumpModelInfo::GetFrameFromId(RpClump *clump, size_t id)
{
    int data[] = {id, NULL};
    RwFrameForAllChildren((RwFrame*)clump->object.parent, FindFrameFromIdCB, data);
    return (RwFrame*)data[1];
}

RwFrame* CClumpModelInfo::FindFrameFromNameCB(RwFrame* frame, void* data)
{
    int* param = (int*)data;
    if (stricmp((const char*)param[0], GetFrameNodeName(frame)) != 0)
    {
        RwFrameForAllChildren(frame, FindFrameFromNameCB, data);
        return param[1] ? NULL : frame;
    }
    else
    {
        param[1] = (int)frame;
        return NULL;
    }
}

RwFrame* CClumpModelInfo::GetFrameFromName(RpClump* clump, char const* name)
{
    void* param[] = {(void*)name, NULL};
    RwFrameForAllChildren(RwFrameGetParent(&clump->object), FindFrameFromNameCB, param);
    return param[1];
}


RwFrame* CClumpModelInfo::FillFrameArrayCB(RwFrame *frame, void *data)
{
    RwFrame** array = (RwFrame**)data;
    int id = CVisibilityPlugins::GetFrameHierarchyId(frame);
    if ( id > 0 )
    {
        array[id] = frame;
    }
    RwFrameForAllChildren(frame, FillFrameArrayCB, data);
    return frame;
}

void CClumpModelInfo::FillFrameArray(RpClump* clump, RwFrame** array)
{
    RwFrameForAllChildren(RwFrameGetParent(&clump->object), FillFrameArrayCB, (void*)array);
}

RpAtomic* CClumpModelInfo::SetAtomicRendererCB(RpAtomic* atomic, void* callback)
{
    CVisibilityPlugins::SetAtomicRenderCallback(atomic, (RpAtomicCallBackRender)callback);
    return atomic;
}

CColModel* CClumpModelInfo::GetBoundingBox()
{
    return m_colModel;
}