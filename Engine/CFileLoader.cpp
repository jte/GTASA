#include "StdInc.h"

RpAtomic *CFileLoader::FindRelatedModelInfoCB(RpAtomic *atomic, void *data)
{
    RpClump *clump = (RpClump*)data;
    const char *name = GetFrameNodeName(GetAtomicFrame(atomic));
    char *modelName;
    bool damage;
    GetNameAndDamage(name, modelName, damage);
    int modelIndex;
    CBaseModelInfo *modelInfo = CModelInfo::GetModelInfo(modelName, modelIndex);
    if(modelInfo)
    {
        CAtomicModelInfo *atomicInfo = modelInfo->AsAtomicModelInfoPtr();
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, NULL);
        if(damage)
        {
#TODO#
            CDamageAtomicModelInfo::SetDamagedAtomic(atomic);
        }
        else
        {
#TODO#
        }
        RpClumpRemoveAtomic(clump, atomic);
        RpAtomicSetFrame(atomic, RwFrameCreate());
        CVisibilityPlugins::SetModelInfoIndex(atomic, modelIndex);
    }
    return atomic;
}

void CFileLoader::LoadAtomicFile(char const *filename)
{
    RwStream *stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, filename);
    if(RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL))
    {
        RpClump *clump = RpClumpStreamRead(stream);
        if(clump)
        {
            RpClumpForAllAtomics(clump, FindRelatedModelInfoCB, clump);
            RpClumpDestroy(clump);
        }
    }
    RwStreamClose(stream, NULL);
}