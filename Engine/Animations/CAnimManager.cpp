#include "StdInc.h"

//#include "AnimAssocDefinitions.inl"

struct KR00
{
    CQuaternion quat;
    float time;
};

struct KRT0
{
    CQuaternion quat;
    CVector trans;
    float time;
};

struct KRTS
{
    CQuaternion quat;
    CVector trans;
    CVector scale;
    float time;
};

struct KR00_Compressed
{
    CompressedQuaternion quat;
    FixedPoint time;
};

struct KRT0_Compressed
{
    CompressedQuaternion quat;
    CompressedVector trans;
    FixedPoint time;
};

struct KRTS_Compressed
{
    CompressedQuaternion quat;
    CompressedVector trans;
    CompressedVector scale;
    FixedPoint time;
};

// v2 - lets the CAnimBlendSequence manage the memory
// v3 - lets the CAnimBlendSequence caller manages the memory
void CAnimManager::ReadANP23(RwStream* stream, bool anp3)
{
    struct
    {
        char block_name[24];
        int32_t animation_count;
    } header;
    RwStreamRead(stream, header.block_name, sizeof(header.block_name));
    RwStreamRead(stream, &header.animation_count, sizeof(header.animation_count));
    CAnimBlock* animBlock = GetAnimationBlock(header.block_name);
    if(animBlock)
    {
        if(animBlock->GetAnimationCount() == 0)
        {
            animBlock->SetAnimationCount(header.animation_count);
            animBlock->SetIdOffset(ms_numAnimations);
        }
    }
    else
    {
        animBlock = &ms_aAnimBlocks[ms_numAnimBlocks++];
        animBlock->SetName(header.block_name);
        animBlock->SetAnimationCount(header.animation_count);
        animBlock->SetIdOffset(ms_numAnimations);
        animBlock->SetFirstAssocGroup(GetFirstAssocGroup(animBlock->GetName()));
    }
    size_t id = animBlock->GetIdOffset();
    animBlock->SetLoaded(true);
    for(size_t i = 0; i < animBlock->GetAnimationCount(); i++)
    {
        CAnimBlendHierarchy* hier = &ms_aAnimations[id++];
        struct 
        {
            char name[24];
            int32_t seq_count;
            int32_t frame_data_size;
            int32_t unk1; // 1 - data is compressed
        } anim;
        RwStreamRead(stream, anim.name, sizeof(anim.name));
        RwStreamRead(stream, &anim.seq_count, sizeof(anim.seq_count));
        unsigned char *st = NULL;
        if(anp3)
        {
            RwStreamRead(stream, &anim.frame_data_size, sizeof(anim.frame_data_size));
            RwStreamRead(stream, &anim.unk1, sizeof(anim.unk1));
            hier->SetLoadCompressed(anim.unk1 & 1);
            st = (unsigned char*)CMemoryMgr::Malloc(anim.frame_data_size);
        }
        hier->SetName(anim.name);
        hier->SetSequences(anim.seq_count);
        hier->m_animBlockId = (animBlock - ms_aAnimBlocks) / sizeof(CAnimBlock);
        hier->bRunningCompressed = 0;
        bool bFirstSeq = true;
        for (size_t j = 0; j < hier->GetNumSequences(); j++)
        {
            CAnimBlendSequence *pSequence = hier->GetSequence(i);
            struct 
            {
                char name[24];
                int32_t type;
                int32_t frames_count;
                int32_t bone_id;
            } seq;
            RwStreamRead(stream, seq.name, sizeof(seq.name));
            RwStreamRead(stream, &seq.type, sizeof(seq.type));
            RwStreamRead(stream, &seq.frames_count, sizeof(seq.frames_count));
            RwStreamRead(stream, &seq.bone_id, sizeof(seq.bone_id));
            pSequence->SetName(seq.name);
            pSequence->SetBoneTag(seq.bone_id);
            if (bFirstSeq)
            {
                bFirstSeq = false;
                hier->SetLoadCompressed(seq.type == 3 || seq.type == 4);
            }
            size_t data_size = 0;
            bool bIsRoot;
            bool bIsCompressed;
            bool bInvalidType = false;
            switch (seq.type)
            {
                case 1:
                    data_size = sizeof(SChildKeyFrame) * seq.frames_count;
                    bIsRoot = false;
                    bIsCompressed = false;
                break;
                case 2:
                    data_size = sizeof(SRootKeyFrame) * seq.frames_count;
                    bIsRoot = true;
                    bIsCompressed = false;
                break;
                case 3:
                    data_size = sizeof(SCompressedChildKeyFrame) * seq.frames_count;
                    bIsRoot = false;
                    bIsCompressed = true;
                break;
                case 4:
                    data_size = sizeof(SCompressedRootKeyFrame) * seq.frames_count;
                    bIsRoot = true;
                    bIsCompressed = true;
                break;
                default:
                    bInvalidType = true;
                break;
            }
            if(!bInvalidType)
            {
                pSequence->SetNumFrames(seq.frames_count, bIsRoot, bIsCompressed, st);
                RwStreamRead(stream, pSequence->pKeyFrames, data_size);
                if(anp3)
                {   
                   st += data_size;
                   pSequence->usFlags |= EXTERNAL_KEYFRAMES_MEM;
                }
            }
        }
        if(!hier->GetLoadCompressed())
        {
            hier->RemoveQuaternionFlips();
            hier->CalcTotalTime();
        }
    }
    if(id > ms_numAnimations)
    {
        ms_numAnimations = id;
    }
}

template<typename _T, int _Boundary>
void Align(_T& v)
{
    if(v % _Boundary != 0)
    {
        v += _Boundary - (v % _Boundary);
    }
}

template<typename _T>
void AlignANP1(_T& v)
{
    Align<_T, 4>(v);
}

void CAnimManager::ReadANP1(RwStream* stream, bool loadCompressed, const char uncompressedAnims[8][32])
{
    struct
    {
        uint32_t eof_offset;
        uint8_t fourcc_info[4];
        uint32_t eoh_offset;
        uint32_t animations_num;
        char block_name[24];
    } header;
    RwStreamRead(stream, &header.eof_offset, sizeof(header.eof_offset));
    AlignANP1(header.eof_offset);
    RwStreamRead(stream, header.fourcc_info, sizeof(header.fourcc_info));
    RwStreamRead(stream, &header.eoh_offset, sizeof(header.eoh_offset));
    AlignANP1(header.eoh_offset);
    RwStreamRead(stream, &header.animations_num, header.eoh_offset);
    CAnimBlock* animBlock = GetAnimationBlock(header.block_name);
    if(animBlock)
    {   // existing animation block
        if(animBlock->GetAnimationCount() == 0)
        {
            animBlock->SetAnimationCount(header.animations_num);
            animBlock->SetIdOffset(ms_numAnimations);
        }
    }
    else
    {   // new animation block
        animBlock = &ms_aAnimBlocks[ms_numAnimBlocks++];
        animBlock->SetName(header.block_name);
        animBlock->SetAnimationCount(header.animations_num);
        animBlock->SetIdOffset(ms_numAnimations);
        animBlock->SetFirstAssocGroup(GetFirstAssocGroup(animBlock->GetName()));
    }
    size_t id = animBlock->GetIdOffset();
    animBlock->SetLoaded(true);
    for(size_t i = 0; i < animBlock->GetAnimationCount(); i++)
    {
        struct 
        {
            uint8_t fourcc_name[4];
            uint32_t anim_name_len;
            char anim_name[1];
            char fourcc_dgan[4];
            uint32_t anim_end_off;
            char fourcc_info[4];
            uint32_t first_seq_off;
            uint32_t num_seq;
            uint32_t pad1;
        } anim;
        RwStreamRead(stream, anim.fourcc_name, sizeof(anim.fourcc_name));
        RwStreamRead(stream, &anim.anim_name_len, sizeof(anim.anim_name_len));
        AlignANP1(anim.anim_name_len);
        RwStreamRead(stream, anim.anim_name, anim.anim_name_len);
        CAnimBlendHierarchy* hier = &ms_aAnimations[id++];
        hier->SetName(anim.anim_name);
        hier->SetLoadCompressed(loadCompressed);
        if(uncompressedAnims)
        {
            for(size_t i = 0; i < ELEMS_COUNT(uncompressedAnims); i++)
            {
                if(uncompressedAnims[i][0] != '\0')
                {
                    if(hier->GetHashKey() == CKeyGen::GetUppercaseKey(uncompressedAnims[i]))
                    {
                        hier->SetLoadCompressed(false);
                    }
                }
            }
        }
        hier->bRunningCompressed = false;
        RwStreamRead(stream, anim.fourcc_dgan, sizeof(anim.fourcc_dgan));
        RwStreamRead(stream, &anim.anim_end_off, sizeof(anim.anim_end_off));
        AlignANP1(anim.anim_end_off);
        RwStreamRead(stream, anim.fourcc_info, sizeof(anim.fourcc_info));
        RwStreamRead(stream, &anim.first_seq_off, sizeof(anim.first_seq_off));
        AlignANP1(anim.first_seq_off);
        RwStreamRead(stream, &anim.num_seq, sizeof(anim.num_seq));
        hier->SetSequences(anim.num_seq);
        for(size_t j = 0; j < hier->GetNumSequences(); j++)
        {
            CAnimBlendSequence* sequence = hier->GetSequence(j);
            struct
            {
                char fourcc_cpan[4];
                size_t seq_end_off;
                char fourcc_anim[4];
                size_t frames_start;
                char seq_name[28];
                size_t frames_count;
                size_t pad;
                size_t next_sibling;
                size_t prev_sibling;
            } seq;
            RwStreamRead(stream, seq.fourcc_cpan, sizeof(seq.fourcc_cpan));
            RwStreamRead(stream, &seq.seq_end_off, sizeof(seq.seq_end_off));
            AlignANP1(seq.seq_end_off);
            RwStreamRead(stream, seq.fourcc_anim, sizeof(seq.fourcc_anim));
            RwStreamRead(stream, &seq.frames_start, sizeof(seq.frames_start));
            AlignANP1(seq.frames_start);
            RwStreamRead(stream, seq.seq_name, seq.frames_start); // WATCH OUT:size=frames_start; OR sizeof(seq_name); DUNNO
            RwStreamRead(stream, &seq.frames_count, sizeof(seq.frames_count));
            RwStreamRead(stream, &seq.pad, sizeof(seq.pad));
            RwStreamRead(stream, &seq.next_sibling, sizeof(seq.next_sibling));
            RwStreamRead(stream, &seq.prev_sibling, sizeof(seq.prev_sibling));
            sequence->SetName(seq.seq_name);
            if(seq.frames_start == 0x2C)
            {
                sequence->SetBoneTag(seq.next_sibling);
            }
            if(seq.pad)
            {
                break;
            }
            for(size_t k = 0; k < seq.frames_count; k++)
            {
                struct 
                {
                    char fourcc[4];
                    size_t end_off;
                } frame;
                RwStreamRead(stream, frame.fourcc, sizeof(frame.fourcc));
                RwStreamRead(stream, &frame.end_off, sizeof(frame.end_off)); // used for a sanity check
                sequence->SetNumFrames(seq.frames_count, strncmp(frame.fourcc, "KR00", 4) != 0, hier->GetLoadCompressed(), NULL);
                // Rotation + Translation + Scale(apprently not used)
                if(strncmp(frame.fourcc, "KRTS", 4) == 0)
                {
                    if(!hier->GetLoadCompressed())
                    {
                        KRTS data;
                        RwStreamRead(stream, &data, sizeof(data));
                        SRootKeyFrame* keyFrame = (SRootKeyFrame*)sequence->GetKeyFrame(k, sequence->GetIsRoot());
                        keyFrame->rotation = data.quat;
                        keyFrame->time = data.time;
                        if(sequence->GetIsRoot())
                        {
                            keyFrame->translation = data.trans;
                        }
                    }
                    else
                    {
                        KRTS_Compressed data;
                        RwStreamRead(stream, &data, sizeof(data));
                        SCompressedRootKeyFrame* keyFrame = (SCompressedRootKeyFrame*)sequence->GetKeyFrame(k, sequence->GetIsRoot());
                        CompressQuaternion(data.quat, keyFrame->rotation);
                        keyFrame->time = FloatToFixedPoint<60>(data.time) + 0.5f;
                        if(sequence->GetIsRoot())
                        {
                            DecompressVector<1024>(data.trans, keyFrame->translation);
                        }
                    }
                }
                // Rotation + Translation
                else if(strncmp(frame.fourcc, "KRT0", 4) == 0)
                {
                    if(!hier->GetLoadCompressed())
                    {
                        KRT0 data;
                        RwStreamRead(stream, &data, sizeof(data));
                        SRootKeyFrame* keyFrame = (SRootKeyFrame*)sequence->GetKeyFrame(k, sequence->GetIsRoot());
                        keyFrame->rotation = data.quat;
                        keyFrame->time = data.time;
                        if(sequence->GetIsRoot())
                        {
                            keyFrame->translation = data.trans;
                        }
                    }
                    else
                    {
                        KRT0_Compressed data;
                        RwStreamRead(stream, &data, sizeof(data));
                        SCompressedRootKeyFrame* keyFrame = (SCompressedRootKeyFrame*)sequence->GetKeyFrame(k, sequence->GetIsRoot());
                        CompressQuaternion(data.quat, keyFrame->rotation);
                        keyFrame->time = FloatToFixedPoint<60>(data.time) + 0.5f;
                        if(sequence->GetIsRoot())
                        {
                            DecompressVector<1024>(data.trans, keyFrame->translation);
                        }
                    }
                }
                // Rotation
                else if(strncmp(frame.fourcc, "KR00", 4) == 0)
                {
                    if(!hier->GetLoadCompressed())
                    {
                        KR00 data;
                        RwStreamRead(stream, &data, sizeof(data));
                        SRootKeyFrame* keyFrame = (SRootKeyFrame*)sequence->GetKeyFrame(k, sequence->GetIsRoot());
                        keyFrame->rotation = data.quat;
                        keyFrame->time = data.time;
                    }
                    else
                    {
                        KR00_Compressed data;
                        RwStreamRead(stream, &data, sizeof(data));
                        SCompressedRootKeyFrame* keyFrame = (SCompressedRootKeyFrame*)sequence->GetKeyFrame(k, sequence->GetIsRoot());
                        CompressQuaternion(data.quat, keyFrame->rotation);
                        keyFrame->time = FloatToFixedPoint<60>(data.time) + 0.5f;
                    }
                }
            }
        }
        if(!hier->GetLoadCompressed())
        {
            hier->RemoveQuaternionFlips();
            hier->CalcTotalTime();
        }
    }
    if(id > ms_numAnimations)
    {
        ms_numAnimations = id;
    }
}

void CAnimManager::LoadAnimFileStream(RwStream* stream, char loadCompressed, const char uncompressedAnims[8][32])
{
    struct
    {
        char FourCC[4];
    } header;
    RwStreamRead(stream, header.FourCC, sizeof(header.FourCC));
    if(strncmp(header.FourCC, "ANP2", sizeof(header.FourCC)) == 0 || strncmp(header.FourCC, "ANP3", sizeof(header.FourCC)) == 0)
    {
        bool anp3 = strncmp(header.FourCC, "ANP3", sizeof(header.FourCC)) == 0;
        ReadANP23(stream, anp3);
    }
    else
    {
        ReadANP1(stream, loadCompressed, uncompressedAnims);
    }
}

CAnimBlock *CAnimManager::GetAnimationBlock(char const *name)
{
    for(size_t i = 0; i < ms_numAnimBlocks; i++)
    {
        if(stricmp(ms_aAnimBlocks[i]->szName, name) == 0)
        {
            return ms_aAnimBlocks[i];
        }
    }
    return NULL;
}

int32_t CAnimManager::GetAnimationBlockIndex(char const *name)
{
    CAnimBlock *pAnimBlock = GetAnimationBlock(name);
    return pAnimBlock ? (pAnimBlock - ms_numAnimBlocks) / sizeof(CAnimBlock): -1;
}

AssocGroupId CAnimManager::GetFirstAssocGroup(char const *block_name);
{
    size_t i = 0;
    for(; i < ELEMS_COUNT(ms_aAnimAssocDefinitions); i++)
    {
        if(stricmp(ms_aAnimAssocDefinitions[i]->szIfpFile, blockName) == 0)
        {
            break;
        }
    }
    return i;
}

CAnimBlendHierarchy *CAnimManager::GetAnimation(unsigned int hash, CAnimBlock const *pAnimBlock)
{
    if(pAnimBlock->GetAnimationCount() == 0)
    {
        return 0;
    }
    size_t id = 0;
    CAnimBlendHierarchy *pAnimHier = NULL;
    do
    {
        pAnimHier = &ms_aAnimations[pAnimBlock->GetIdOffset() + id];
        id++;
    } 
    while(id < pAnimBlock->GetAnimationCount() && pAnimHier->GetHashKey() != hash);
    return pAnimHier;
}

CAnimManager::AnimAssocDefinition *CAnimManager::AddAnimAssocDefinition(char const *group, char const *file, unsigned int type, unsigned int count, CAnimManager::AnimDescriptor *descriptor)
{
    // Find free slot
    size_t i = ms_numAnimAssocDefinitions;
    while(ms_aAnimAssocDefinitions[i].name[0] != '\0')
    {
        i++;
    }
    // Fill out assoc definition
    ms_numAnimAssocDefinitions = i;
    AnimAssocDefinition *pDef = &ms_aAnimAssocDefinitions[ms_numAnimAssocDefinitions];
    strncpy(pDef->szGroupName, group, sizeof(pDef->szGroupName));
    strncpy(pDef->szIfpFile, file, sizeof(pDef->szIfpFile));
    pDef->iGroupAnimType = type;
    pDef->iNumAnims = count;
    pDef->pDescriptor = *descriptor;
    pDef->pszAnimations = new char[pDef->iNumAnims];
    // CHECK: Do we need to mark next slot as free?
}

AnimAssocDefinition *CAnimManager::GetAnimGroupName(AssocGroupId index)
{
    return ms_aAnimAssocDefinitions[index].szGroupName;
}

const char *CAnimManager::GetAnimBlockName(AssocGroupId index)
{
    return ms_aAnimAssocDefinitions[index].szBlockName;
}

CAnimBlendAssociation *CAnimManager::GetAnimAssociation(AssocGroupId group, AnimationId anim)
{
    return ms_aAnimAssocGroups[group].GetAnimation(anim);
}

CAnimBlendAssociation *CAnimManager::GetAnimAssociation(AssocGroupId group, char const *name)
{
    return ms_aAnimAssocGroups[group].GetAnimation(name);
}

void CAnimManager::AddAnimToAssocDefinition(CAnimManager::AnimAssocDefinition *pDef, char const *name)
{
    char **pAnims = pDef->pszAnimations;
    while(*pAnims[0] != '\0')
    {
        pAnims++;
    }
    strncpy(*pAnims, name, 24);
}

int32_t CAnimManager::RegisterAnimBlock(char const *block_name)
{
    CAnimBlock *pAnimBlock = GetAnimationBlock(block_name);
    if(pAnimBlock == NULL)
    {
        pAnimBlock = &ms_aAnimBlocks[ms_numAnimBlocks];
        pAnimBlock->SetName(block_name);
        pAnimBlock->SetAnimationCount(0);
        pAnimBlock->SetFirstAssocGroup(GetFirstAssocGroup(pAnimBlock->GetName()));
        ms_numAnimBlocks++;
    }
    return (pAnimBlock - ms_aAnimBlocks) / sizeof(pAnimBlock);
}

void CAnimManager::RemoveAnimBlock(int index)
{
    CAnimBlock *pAnimBlock = &ms_aAnimBlocks[index];
    for(size_t i = 0; i < ms_numAnimAssocDefinitions; i++)
    {
        if(ms_aAnimAssocGroups[i]->pAnimBlock == pAnimBlock)
        {
            ms_aAnimAssocGroups[i]->DestroyAssociations();
        }
    }
    for(size_t i = 0; i < pAnimBlock->GetAnimationCount(); i++)
    {
        ms_aAnimations[i + pAnimBlock->iIDOffset]->Shutdown();
    }
    pAnimBlock->SetLoaded(false);
    pAnimBlock->SetReferenceCount(0);
}

void CAnimManager::AddAnimBlockRef(int index)
{
    ms_aAnimBlocks[index].usRefs++;
}

void CAnimManager::RemoveAnimBlockRef(int index)
{
    ms_aAnimBlocks[index].usRefs--;
}

void CAnimManager::RemoveAnimBlockRefWithoutDelete(int index)
{
    ms_aAnimBlocks[index].usRefs--;
}

uint16_t CAnimManager::GetNumRefsToAnimBlock(int index)
{
    return ms_aAnimBlocks[index].usRefs;
}

CAnimBlendHierarchy *CAnimManager::GetAnimation(char const *name, CAnimBlock const *pAnimBlock)
{
    if(pAnimBlock->iNumAnimations == 0)
    {
        return 0;
    }
    size_t id = pAnimBlock->iIDOffset;
    size_t hash = CKeygen::GetUppercaseKey(name);
    CAnimBlendHierarchy *pAnimHier = NULL;
    do
    {
        pAnimHier = ms_aAnimations[id++];
    } 
    while(pAnimHier->iHashKey != hash);
    return pAnimHier;
}

void CAnimManager::RemoveLastAnimFile()
{
    CAnimBlock *pAnimBlock = ms_aAnimBlocks[ms_numAnimBlocks - 1];
    for(size_t i = 0; i < pAnimBlock->iNumAnimations; i++)
    {
        ms_aAnimations[pAnimBlock->iIDOffset + i]->Shutdown();
    }
    ms_numAnimations = pAnimBlock->iIDOffset;
    pAnimBlock->bLoaded = false;
    ms_numAnimBlocks--;
}

void CAnimManager::CreateAnimAssocGroups()
{
    for(size_t i = 0; i < ms_numAnimAssocDefinitions; i++)
    {
        AnimAssocDefinition *pDef = ms_aAnimAssocDefinitions[i];
        CAnimBlock *pAnimBlock = NULL;
        for(size_t j = 0; j < ms_numAnimBlocks; j++)
        {
            if(stricmp(ms_aAnimBlocks[j]->szName, pDef->szIfpFile) == 0)
            {
                pAnimBlock = ms_aAnimBlocks[j];
                break;
            }
        }
        if(pAnimBlock && pAnimBlock->bLoaded)
        {
            CAnimBlendAssocGroup *pAssocGroup = ms_aAnimAssocGroups[i];
            RpClump *pAnimClump = NULL;
            if(pAssocGroup->pAssociationsArray == NULL)
            {
                if(pDef->anim_type != -1)
                {
                    pAnimClump = RpAnimBlendClumpInit(CModelInfo::ms_modelInfoPtrs[pDef->anim_type]->CreateInstance());
                }
            }
            pAssocGroup->groupId = i;
            pAssocGroup->iIDOffset = *pDef->descriptor->iIDOffset;
            pAssocGroup->CreateAssociations(pDef->szIfpFile, pAnimClump, pDef->pszAnimations, pDef->iNumAnims);
            for(size_t k = 0; k < pAssocGroup->iNumAnimations; k++)
            {
                CAnimBlendStaticAssociation *pStaticAssoc = pAssocGroup->GetAnimation(pDef->pDescriptor->AnimAssoc[k].uiStaticAssocId);
                pStaticAssoc->usFlags |= pDef->pDescriptor->AnimAssoc[k].usFlags;
            }
            if(pAnimClump)
            {
                RpAtomic *pAtomic = GetFirstAtomic(pAnimClump);
                if(pAtomic)
                {
                    if(RpSkinGeometryGetSkin(pAtomic->geometry) != 0)
                    {
                        RpClumpForAllAtomics(pAnimClump, AtomicRemoveAnimFromSkinCB, 0);
                    }
                }
                RpClumpDestroy(pAnimClump);
            }
        }
    }
}

CAnimBlendAssociation *CAnimManager::CreateAnimAssociation(AssocGroupId group, AnimationId anim)
{
    return ms_aAnimAssocGroups[group]->CopyAnimation(anim);
}

void CAnimManager::UncompressAnimation(CAnimBlendHierarchy *pHierarchy)
{
    if(pHierarchy->bRunningCompressed)
    {
        if(pHierarchy->fTotalTime == 0.0)
        {
            pHierarchy->CalcTotalTimeCompressed();
        }
    }
    else
    {
        if(pHierarchy->bLoadCompressed)
        {
            CLinkList *pCache = ms_animCache.Insert(pHierarchy);
            if(pCache == NULL)
            {
                // 
            }
            pHierarchy->pLinkPtr = pCache;
        }
        else
        {
        }
    }
}

void CAnimManager::Shutdown()
{
    for(size_t i = 0; i < 180; i++)
    {
        CModelInfo::RemoveModel(25575 + i);
    }
    for(size_t i = 0; i < ms_numAnimations; i++)
    {
        ms_aAnimations[i]->Shutdown();
    }
    delete ms_animCache.field_30;
    ms_animCache.field_30 = NULL;
    ms_aAnimAssocGroups.clear();
}

void CAnimManager::RemoveFromUncompressedCache(CAnimBlendHierarchy *pHierarchy)
{
    CLinkList<CAnimHierarchy>* pLinkPtr = pHierarchy->pLinkPtr;
    if(pLinkPtr)
    {
        pLinkPtr->next->prev = pLinkPtr->prev;
        pLinkPtr->prev->next = pLinkPtr->next;
        pLinkPtr->next = &ms_animCache.freeList_Head;
        ms_animCache.freeList_Head->prev = pLinkPtr;
        pLinkPtr->prev = &ms_animCache.field_18;
        ms_animCache.freeList_Head = &pLinkPtr;
        pHierarchy->pLinkPtr = NULL;
    }
}

void CAnimManager::BlendAnimation(RpClump *pClump, CAnimBlendHierarchy *pAnimHierarchy, int flags, float fBlendDelta)
{
    bool bFound = false;
    if(pClumpAnimAssociations)
    {
        CAnimBlendAssociation *pAssoc = RpAnimBlendClumpGetFirstAssociation(pClump);
        CAnimBlendAssociation *pThisAssoc = NULL;
        for(; pAssoc; pAssoc = RpAnimBlendGetNextAssociation(pAssoc))
        {
            if(pAssoc == this)
            {
                pThisAssoc = pAssoc;
            }
            else
            {
                if((pAssoc->ClumpAnimAssoc.usFlags & 0x10) == (flags & 0x10))
                {
                    if(pAssoc->ClumpAnimAssoc.fBlendAmount <= 0.0)
                    {
                        pAssoc->ClumpAnimAssoc.fBlendDelta = -1.0;
                    }
                    else if(!(pAssoc->ClumpAnimAssoc.fBlendAmount * fBlendDelta <= -pAssoc->ClumpAnimAssoc.fBlendDelta) || !(flags & 0x10) || (pAssoc->pAnimHierarchy->iAnimBlockID == m_iAnimBlockID && pAssoc->pAnimHierarchy->iAnimBlockID >= 0))
                        {
                            float fBlend = -pAssoc->ClumpAnimAssoc.fBlendAmount * fBlendDelta;
                            if(fBlend > -0.05)
                            {
                                fBlend = -0.05;
                            }
                            pAssoc->ClumpAnimAssoc.fBlendDelta = fBlend;
                        }
                    }
                    pAssoc->ClumpAnimAssoc.usFlags |= 4;
                    bFound = true;
                }
            }
        }
        if(pThisAssoc)
        {
            pThisAssoc->SetBlendTo(1.0, fBlendDelta);
            if(pThisAssoc->pAnimHierarchy->fTotalTime == pThisAssoc->fCurrentTime)
            {
                pThisAssoc->Start(0.0);
            }
            CAnimManager::UncompressAnimation(pThisAssoc->pAnimHierarchy);
            return pThisAssoc;
        }
    }
    CAnimBlendAssociation *pAssoc = new CAnimBlendAssociation(pClump, this);
    pAssoc->ReferenceAnimBlock();
    UncompressAnimation();
    RpAnimBlendClumpAddAssociation(pClump, pAssoc, flags, 0.0, 1.0);
    if(bFound || (flags & 0x10))
    {
        pAssoc->SetBlend(0.0, fBlendDelta);
        pAssoc->pAnimHierarchy->UncompressAnimation();
    }
    return pAssoc;
}

void CAnimManager::BlendAnimation(RpClump *pClump, AssocGroupId groupId, AnimationId animId, float fBlendDelta)
{
    CAnimBlendClumpData *pAnimBlendClumpData = *(pClump + ClumpOffset);
    CAnimBlendStaticAssociation *pStaticAssoc = ms_aAnimAssocGroups[groupId]->GetAnimation(animId);
    SClumpAnimAssoc *pAssoc = pAnimBlendClumpData->pFirstAnimAssoc;
    CAnimBlendAssociation *pSyncRefAnimAssoc;
    bool bFound = false;
    if(pAssoc)
    {
        CAnimBlendAssociation *pFoundAnimAssoc = NULL;
        do
        {   
            pAnimAssoc = pAssoc[-1];
            if(pStaticAssoc->usFlags & 0x20)
            {
                if(pAnimAssoc->ClumpAnimAssoc.usFlags & 0x20)
                {
                    pSyncRefAnimAssoc = pAnimAssoc;
                }
            }
            if(pAnimAssoc->ClumpAnimAssoc.usAnimId == animId && pAnimAssoc->ClumpAnimAssoc.usAnimGroup == groupId)
            {
                pFoundAnimAssoc = pAnimAssoc;
            }
            else
            {
                if((pAnimAssoc->ClumpAnimAssoc.usFlags & 0x10) == (pStaticAssoc->usFlags & 0x10))
                {
                    if(((pAnimAssoc->ClumpAnimAssoc.usFlags & 0x8000) && (pAnimAssoc->ClumpAnimAssoc.usFlags & 0x8000)) || !(pAnimAssoc->ClumpAnimAssoc.usFlags & 0x8000))
                    {
                        if(pAnimAssoc->ClumpAnimAssoc.fBlendAmount <= 0.0)
                        {
                            pAnimAssoc->ClumpAnimAssoc.fBlendDelta = -1.0;
                        }
                        else
                        {
                            float fBlend = -pAnimAssoc->ClumpAnimAssoc.fBlendAmount * fBlendDelta;
                            if(fBlend < pAnimAssoc->ClumpAnimAssoc.fBlendDelta || !(pStaticAssoc->usFlags & 0x10))
                            {
                                if(fBlend > -0.05)
                                {
                                    fBlend = -0.05;
                                }
                                pAnimAssoc->ClumpAnimAssoc.fBlendDelta = fBlend;
                            }
                        }
                        pAnimAssoc->ClumpAnimAssoc.usFlags |= 0x04;
                        bFound = true;
                    }
                }
            }
            pAssoc = pAssoc->pNext;
        } while(pAssoc);
        if(pFoundAnimAssoc)
        {
            pFoundAnimAssoc->SetBlendTo(1.0, fBlendDelta);
            if(pFoundAnimAssoc->pAnimHierarchy->fTotalTime == pFoundAnimAssoc->fCurrentTime)
            {
                pFoundAnimAssoc->Start(0.0);
            }
            UncompressAnimation(pFoundAnimAssoc->ClumpAnimAssoc.pAnimHierarchy);
            return pFoundAnimAssoc;
        }
    }
    CAnimBlendAssociation *pNewAnimAssoc = AddAnimationAndSync(pClump, pSyncRefAnimAssoc, groupId, animId);
    if(bFound || v5)
    {   
        pNewAnimAssoc->ClumpAnimAssoc.fBlendAmount = 0.0;
        pNewAnimAssoc->ClumpAnimAssoc.fBlendDelta = fBlendDelta;
        UncompressAnimation(pNewAnimAssoc->ClumpAnimAssoc.pAnimHierarchy);
    }
    else
    {
        pNewAnimAssoc->ClumpAnimAssoc.fBlendAmount = 1.0;
    }
    return pNewAnimAssoc;
}

void CAnimManager::AddAnimationAndSync(RpClump *pClump, CAnimBlendAssociation *pAnimAssocToSync, AssocGroupId groupId, AnimationId animId)
{
    CAnimBlendAssociation *pAnimAssoc = CreateAnimAssociation(groupId, animId);
    CAnimBlendClumpData *pAnimBlendClumpData = *(pClump + ClumpOffset);
    if((pAnimAssoc->ClumpAnimAssoc.usFlags & 0x20) && pAnimAssocToSync)
    {
        pAnimAssoc->SyncAnimation(pAnimAssocToSync);
        pAnimAssoc->ClumpAnimAssoc.usFlags |= 0x01;
    }
    else
    {
        pAnimAssoc->Start(0.0);
    }
    if(pAnimBlendClumpData->ppAnimAssociations)
    {
        pAnimBlendClumpData->pUserPtr = &pAnimAssoc->ClumpAnimAssoc;
    }
    pAnimAssoc->ClumpAnimAssoc.pUserPtr = pAnimBlendClumpData;
    pAnimAssoc->ClumpAnimAssoc->pNext = pAnimBlendClumpData->ppAnimAssociations;
    pAnimBlendClumpData->ppAnimAssociations = &pAnimAssoc->ClumpAnimAssoc;
    
}