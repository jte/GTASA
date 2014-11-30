#include "StdInc.h"
/*
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
}*/

const char* CFileLoader::LoadLine(int fp)
{
    if(!CFileMgr::ReadLine((FILE*)fp, ms_line, sizeof(ms_line)))
    {
        return NULL;
    }
    char* p = ms_line;
    while(p)
    {
        if(*p < ' ' || *p == ',')
        {
            *p = ' ';
        }
    }
    p = ms_line;
    while(p && *p == ' ') p++;
    return p;
}

bool CFileLoader::LoadClumpFile(RwStream *stream, size_t model_id)
{
    char v2; // bl@1
    int v3; // ebp@1
    CVehicleModelInfo *v4; // esi@1
    RpClump *v5; // ebx@2
    RwFrame *v6; // ebp@2
    RpClump *v7; // eax@3
    RpClump *v8; // esi@3
    RwFrame *v9; // eax@4
    RpClump *v11; // edi@10
    CVehicleModelInfo *v12; // [sp+10h] [bp+10h]@1

    v3 = model_id;
    CVehicleModelInfo* v12 = static_cast<CVehicleModelInfo*>(CModelInfo::GetModelInfo(model_id));
    v4 = v12;
    v2 = (unsigned __int8)v12->__parent.__parent.__vmt->getType(v12) == 6;
    if ( HIBYTE(v12->__parent.__parent.flags) & 2 )
    {
        v5 = (RpClump *)RpClumpCreate();
        v6 = RwFrameCreate();
        v5->object.parent = v6;
        if ( !v7ed2d0(stream, 16, 0, 0) )
        {
        LABEL_6:
            ((void (__thiscall *)(CVehicleModelInfo *, RpClump *))v4->__parent.__parent.__vmt[1]._m04)(v4, v5);
            return 1;
        }
        while ( 1 )
        {
            v7 = RpClumpStreamRead(stream);
            v8 = v7;
            if ( !v7 )
            return 0;
            v9 = _rwFrameCloneAndLinkClones(v7->object.parent);
            v7f0b00(v6, v9);
            RpClumpForAllAtomics(v8, (int (__cdecl *)(_DWORD, _DWORD))dummy_537290, v5);
            RpClumpDestroy(v8);
            if ( !v7ed2d0(stream, 16, 0, 0) )
            {
                v4 = v12;
                goto LABEL_6;
            }
        }
    }
    if ( !v7ed2d0(stream, 16, 0, 0) )
    return 0;
    if ( v2 )
    {
    CCollisionPlugin__SetModelInfo(&v12->__parent.__parent);
    CVehicleModelInfo__UseCommonVehicleTexDicationary();
    }
    v11 = RpClumpStreamRead(stream);
    if ( v2 )
    {
    CCollisionPlugin__SetModelInfo(0);
    restoreTextureFindCallback();
    }
    if ( !v11 )
    return 0;
    ((void (__thiscall *)(CVehicleModelInfo *, RpClump *))v12->__parent.__parent.__vmt[1]._m04)(v12, v11);
    if ( v3 == 508 )
    v12->ucDoorCount = 2;
    return 1;
}