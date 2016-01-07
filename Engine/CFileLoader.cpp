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
    if(!CFileMgr::ReadLine(fp, ms_line, sizeof(ms_line)))
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

void CFileLoader::LoadObjectTypes(const char *filename)
{
    int fp = CFileMgr::OpenFile(filename, "rb");
    const char* line;
    int32_t path;
    size_t in_section = 0; /* section id */
    struct 
    {
        size_t section;
        char name[5];
    } sections[] = 
    {
        {0, "end"},
        {1, "objs"},
        {2, "tobj"},
        {3, "weap"},
        {4, "hier"},
        {5, "anim"},
        {6, "cars"},
        {7, "peds"}, 
        {8, "path"},
        {9, "2dfx"},
        {10,"txdp"}
    };
    while ((line = CFileLoader::LoadLine(fp)))
    {
        /* skip empty lines and comments */
        if (line[0] == '\0' || line[0] == '#')
        {
            continue;
        }
        if (in_section == 0)
        {
            for (size_t i = 0; i < ELEMS_COUNT(sections); i++)
            {
                if (!strncmp(line, sections[i].name, strlen(sections[i].name)))
                {
                    in_section = sections[i].section;
                }
            }
            /* if we changed section skip the current line which only denotes section name */
            if (in_section != 0)
            {
                line = CFileLoader::LoadLine((int)fp);
            }
        }
        switch (in_section)
        {
            case 1:
                LoadObject(line);
            break;
            case 2:
                LoadTimeObject();
            break;
            case 3:
                LoadWeaponObject();
            break;
            case 4:
                LoadClumpObject();
            break;
            case 5:
                LoadAnimatedClumpObject();
            break;
            case 6:
                LoadVehicleObject();
            break;
            case 7:
                LoadPedObject();
            break;
            case 8:
                static size_t path_type = -1;
                if (path == -1)
                {
                    path = 0;
                    LoadPathHeader(line, &path_type);
                }
                else
                {
                    switch (path_type)
                    {
                        case 0:
                            LoadPedPathNode();
                        break;
                        case 1:
                            LoadCarPathNode();
                        break;
                        case 2:
                            LoadCarPathNode();
                        break;
                    }
                    path++;
                    if (path == 12)
                    {
                        path = -1;
                    }
                }
            break;
            case 9:
                Load2dEffect();
            break;
            case 10:
                LoadTXDParent();
            break;
            default:
                continue;
            break;
        }
    }
    CFileMgr::CloseFile(fp);
}

static uint32_t relatedModelIndex;

bool CFileLoader::LoadAtomicFile(RwStream* stream, uint32_t modelIndex)
{
    CAtomicModelInfo* info = CModelInfo::GetModelInfo(modelIndex)->AsAtomicModelInfoPtr();
    v4 = 0;
    if (info && info->flags & 0x8000)
    {
        v4 = 1;
        CVehicleModelInfo::UseCommonVehicleTexDicationary();
    }
    if (RwStreamFindChunk(stream, 16, NULL, NULL))
    {
        RpClump* read = RpClumpStreamRead(stream);
        if (!read)
        {
            if (v4)
            {
                CVehicleModelInfo::StopUsingCommonVehicleTexDicationary();
            }
            return false;
        }
        relatedModelIndex = modelIndex;
        RpClumpForAllAtomics(read, SetRelatedModelInfoCB, read);
        RpClumpDestroy(read);
    }
    if (!info->GetRwObject())
    {
        return false;
    }
    if (v4)
    {
        CVehicleModelInfo::StopUsingCommonVehicleTexDicationary();
    }
    return true;
}

RpAtomic* CFileLoader::SetRelatedModelInfoCB(RpAtomic* atomic, void* data)
{
    RpClump* clump = (RpClump*)data;

    CAtomicModelInfo* atomicModelInfo = (CAtomicModelInfo *)CModelInfo::GetModelInfo(relatedModelIndex)->AsAtomicModelInfoPtr();
    const char* frameNodeName = GetFrameNodeName(RwFrameGetParent(atomic));
    bool damaged;
    GetNameAndDamage(frameNodeName, (char *)&v10, (int)&damaged);
    CVisibilityPlugins::SetAtomicRenderCallback(atomic, 0);
    if (damaged)
    {
        CDamageAtomicModelInfo* damageModel = atomicModelInfo->AsDamageAtomicModelInfoPtr();
        damageModel->SetDamagedAtomic(atomic);
    }
    else
    {

    ((void (__thiscall *)(CAtomicModelInfo *, RpAtomic *))v5->__parent.__vmt[1].__destructor)(v5, v4);
    }
    RpClumpRemoveAtomic(clump, atomic);
    RwFrame* frame = RwFrameCreate();
    RpAtomicSetFrame(atomic, frame);
    CVisibilityPlugins::SetModelInfoIndex(atomic, relatedModelIndex);
    return atomic;
}