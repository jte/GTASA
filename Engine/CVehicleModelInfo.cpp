#include "StdInc.h"

static RwTexture* gpWhiteTexture;
static RwTextureCallBackFind gDefaultTextureFindCB;
static RwTexDictionary* gVehicleTxd;
static EditableMaterial gVehicleEditableMaterials[256];

void CVehicleModelInfo::Init()
{
    CClumpModelInfo::Init();
    m_vehicleType = -1;
    m_wheelModelId = -1;
    m_steerAngle = 999.99;
}

RpMaterial* CVehicleModelInfo::HasAlphaMaterialCB(RpMaterial* material, void* data)
{
    if (material->color.alpha != 255)
    {
        bool* hasAlpha = (bool*)data;
        *hasAlpha = true;
        return NULL;
    }
    return material;
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB(RpAtomic* atomic, void*)
{
    const char* nodeName = GetFrameNodeName(RwFrameGetParent(&atomic->object.object));
    RpAtomicCallBackRender renderCB;
    if (strstr(nodeName, "_vlo"))
    {
        renderCB = CVisibilityPlugins::RenderVehicleReallyLowDetailCB;
    }
    else
    {
        bool hasAlphaMaterial = false;
        RpGeometryForAllMaterials(atomic->geometry, HasAlphaMaterialCB, &hasAlphaMaterial);
        if (!hasAlphaMaterial && strncmp(nodeName, "windscreen", 10) != 0)
        {
            renderCB = CVisibilityPlugins::RenderVehicleHiDetailCB;
        }
        else
        {
            renderCB = CVisibilityPlugins::RenderVehicleHiDetailAlphaCB;
        }
    }
    CVisibilityPlugins::SetAtomicRenderCallback(atomic, renderCB);
    HideDamagedAtomicCB(atomic);
    return atomic;
}

RpAtomic* CVehicleModelInfo::HideDamagedAtomicCB(RpAtomic* atomic,void*)
{
    RwFrame* frame = GetAtomicFrame(atomic);
    const char* nodeName = GetFrameNodeName(frame);
    if (strstr(nodeName, "_dam"))
    {
        atomic->object.object.flags = 0;
        CVisibilityPlugins::SetAtomicFlag(atomic, 2);
    }
    else if (strstr(nodeName, "_ok"))
    {
        CVisibilityPlugins::SetAtomicFlag(atomic, 1);
    }
    return atomic;
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_RealHeli(RpAtomic* atomic, void*)
{
    const char* nodeName = GetFrameNodeName(GetAtomicFrame(atomic));
    if (!strcmp(nodeName, "moving_rotor"))
    {
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderHeliRotorAlphaCB);
    }
    else if (!strcmp(nodeName, "moving_rotor2"))
    {
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderHeliTailRotorAlphaCB);
    }
    else if (strstr(nodeName, "_vlo") != NULL)
    {
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleReallyLowDetailCB);
    }
    else
    {
        bool hasAlphaMaterial = false;
        RpGeometryForAllMaterials(atomic->geometry, HasAlphaMaterialCB, &hasAlphaMaterial);
        if (hasAlphaMaterial || !strncmp(nodeName, "windscreen", 10))
        {
            CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleHiDetailAlphaCB);
        }
        else
        {
            CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleHiDetailCB);
        }
    }
    HideDamagedAtomicCB(atomic, NULL);
    return atomic;
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Boat(RpAtomic* atomic)
{
    const char* nodeName = GetFrameNodeName(GetAtomicFrame(atomic));
    if ( !strcmp(nodeName, "boat_hi") )
    {
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleHiDetailCB_Boat);
    }
    else if (!strstr(nodeName, "_vlo"))
    {
        bool hasAlphaMaterial = false;
        RpGeometryForAllMaterials(atomic->geometry, HasAlphaMaterialCB, &hasAlphaMaterial);
        if (hasAlphaMaterial)
        {
            CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleHiDetailAlphaCB_Boat);
        }
        else
        {
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleHiDetailCB_Boat);
        }
    }
    else
    {
        CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderVehicleLoDetailCB_Boat);
    }
    HideDamagedAtomicCB(atomic, 0);
    return atomic;
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_BigVehicle(RpAtomic* atomic,void*)
{
    const char* nodeName = GetFrameNodeName(GetAtomicFrame(atomic));
    RpAtomicCallBackRender callback;
    if (strstr(nodeName, "_vlo"))
    {
        callback = CVisibilityPlugins::RenderVehicleReallyLowDetailCB_BigVehicle;
    }
    else
    {
        bool hasAlphaMaterial = false;
        RpGeometryForAllMaterials(atomic->geometry, HasAlphaMaterialCB, &hasAlphaMaterial);
        if (hasAlphaMaterial)
        {
            callback = CVisibilityPlugins::RenderVehicleHiDetailAlphaCB_BigVehicle;
        }
        else
        {
            callback = CVisibilityPlugins::RenderVehicleHiDetailCB_BigVehicle;
        }
    }
    CVisibilityPlugins::SetAtomicRenderCallback(atomic, callback);
    HideDamagedAtomicCB(atomic, NULL);
    return atomic;
}

void CVehicleModelInfo::SetAtomicRenderCallbacks()
{
    RpAtomicCallBack callback;
    switch (m_VehicleType)
    {
        case 6:
            callback = SetAtomicRendererCB_Train;
        break;
        case 8: case 4:
            callback = SetAtomicRendererCB_BigVehicle;
        break;
        case 5:
            callback = SetAtomicRendererCB_Boat;
        break;
        case 3:
            callback = SetAtomicRendererCB_RealHeli;
        break;
        default:
            callback = CVehicleModelInfo::SetAtomicRendererCB;
        break;
    }
    RpClumpForAllAtomics((RpClump*)m_rwObject, callback, NULL);
}

RpAtomic* CVehicleModelInfo::HideAllComponentsAtomicCB(RpAtomic* atomic, void* data)
{
    atomic->object.object.flags = ((*(unsigned int*)data) & CVisibilityPlugins::GetAtomicId(atomic)) == 0 ? 4 : 0;
}

static const size_t gWheelIdToFrameId[] = {5, 7, 2, 4};

void CVehicleModelInfo::GetWheelPosn(int id, CVector& out, bool dontTransform)
{
    RwFrame* frame = GetFrameFromId((RpClump*)m_rwObject, gWheelIdToFrameId[id]);
    if (m_vehicleType != 4 || dontTransform)
    {
        out = frame->modelling.pos;
    }
    else
    {
        RwMatrix* matrix = RwMatrixCreate();
        *matrix = frame->modelling;
        for (RwFrame* i = (RwFrame*)frame->object.parent; i; i = (RwFrame*)i->object.parent)
        {
            RwMatrixTransform(matrix, &i->modelling, rwCOMBINEPOSTCONCAT);
        }
        out = matrix->pos;
        RwMatrixDestroy(matrix);
    }
}

bool CVehicleModelInfo::GetOriginalCompPosition(CVector& out, int id)
{
    RwFrame* frame = GetFrameFromId((RpClump*)m_rwObject, id);
    if (!frame)
    {
        return false;
    }
    out = frame->modelling.pos;
    return true;
}

static RpMaterial* RemoveWindowAlphaCB(RpMaterial *material, void* data)
{
	EditableMaterial** editMatArray = (EditableMaterial**)data;

	if (material->color.alpha < 255)
	{
		(*editMatArray)->material = material;
		(*editMatArray)->texture = material->texture;
		(*editMatArray)++;
		material->color.red = 0;;
		material->color.green = 0;
		material->color.blue = 0;
		material->color.alpha = 0;
	}
	return material;
}

RpAtomic* CVehicleModelInfo::SetEditableMaterialsCB(RpAtomic* atomic, void* data)
{
    if (RpAtomicGetFlags(atomic) & 4)
    {
        if (CVisibilityPlugins::GetAtomicId(atomic) & 0x1000)
        {
            RpGeometryForAllMaterials(atomic->geometry, RemoveWindowAlphaCB, data);
        }
        RpGeometryForAllMaterials(atomic->geometry, SetEditableMaterialsCB, data);
    }
    return atomic;
}

void CVehicleModelInfo::SetVehicleColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    ms_currentCol = CRGBA(r, g, b, a);
    m_colour = CRGBA(r, g, b, a);
}

size_t CVehicleModelInfo::GetNumRemaps()
{
    size_t i;
    for (i = 0; i < ELEMS_COUNT(m_remaps); i++)
    {
        if (m_remaps[i] == -1)
        {
            break;
        }
    }
    return i;
}

void CVehicleModelInfo::AddRemap(int16_t id)
{
    m_remaps[GetNumRemaps()] = id;
}

void CVehicleModelInfo::AddWheelUpgrade(uint32_t wheelSet, int16_t modelId)
{
    ms_upgradeWheels[ms_numWheelUpgrades[wheelSet]][wheelSet] = modelId;
    ms_numWheelUpgrades[wheelSet]++;
}

void CVehicleModelInfo::SetClump(RpClump* clump)
{
    m_vehicleStruct = new CVehicleStructure; 
    CClumpModelInfo::SetClump(clump);
    SetAtomicRenderCallbacks();
    CClumpModelInfo::SetFrameIds(&ms_vehicleDescs[m_vehicleType]);
    SetRenderPipelines();
    PreprocessHierarchy();
    ReduceMaterialsInVehicle();
    m_colour = CRGBA(255, 255, 255, 255);
    SetCarCustomPlate();
}

static RwFrame* gVehicleEnvMapFrame = NULL;

void CVehicleModelInfo::SetRenderPipelines()
{
    CCarFXRenderer::CustomCarPipeClumpSetup((RpClump*)m_rwObject);
    if (!gVehicleEnvMapFrame)
    {
        gVehicleEnvMapFrame = RwFrameCreate();
        RwV3d axis;
        axis.x = 1.0f;
        axis.y = 0.0f;
        axis.z = 0.0f;
        RwMatrixRotate(&gVehicleEnvMapFrame->modelling, &axis, 60.0f, rwCOMBINEREPLACE);
        RwFrameUpdateObjects(gVehicleEnvMapFrame);
        //RwFrameGetLTM(gVehicleEnvMapFrame);
    }
}

void CVehicleModelInfo::PreprocessHierarchy()
{
    RwObjectNameIdAssocation* assoc = &ms_vehicleDescs[m_vehicleType];
    if (!assoc[0].name)
    {
        return;
    }
    m_numDoors = 0;
    RpAtomic *v49, *atomic;
    for (size_t i = 0; assoc[i + 1].name; i++)
    {
        if (assoc[i].flags & 0x20208)
        {
            void* params[] = {assoc[i].name, NULL};
            RwFrameForAllChildren(RwFrameGetParent(m_rwObject), FindFrameFromNameWithoutIdCB, params);
            if (params[1])
            {
                RwFrame* frame = (RwFrame*)params[1];
                if (assoc[i].flags & 8)
                {
                    RwV3d pos = m_vehicleStruct->m_dummyPos[assoc[i].hierarchyID];
                    pos = frame->modelling.pos;
                    RwFrame* iter = RwFrameGetParent(frame);
                    if (iter && iter->object.parent)
                    {
                        do
                        {
                            RwV3dTransformPoints(&pos, &pos, 1, &iter->modelling);
                            iter = RwFrameGetParent(iter);
                        }
                        while (iter->object.parent);
                    }
                    m_vehicleStruct->m_dummyPos[assoc[i].hierarchyID] = pos;
                    RwFrameDestroy(frame);
                }
                else if (assoc[i].flags & 0x20000)
                {
                    m_vehicleStruct->m_upgrades[assoc[i].hierarchyID].m_position = frame->modelling.pos;
                    m_vehicleStruct->m_upgrades[assoc[i].hierarchyID].m_rotation.Set(frame->modelling);
                    m_vehicleStruct->m_upgrades[assoc[i].hierarchyID].m_parentHierarchyId = CVisibilityPlugins::GetFrameHierarchyId(RwFrameGetParent(frame));
                }
                else
                {
                    RpAtomic *atomic = (RpAtomic*)GetFirstObject(frame);
                    RpClumpRemoveAtomic((RpClump*)m_rwObject, atomic);
                    RwFrameRemoveChild(frame);
                    SetVehicleComponentFlags(frame, assoc[i].flags);
                    m_vehicleStruct->m_extras[m_vehicleStruct->m_numExtras] = atomic;
                    m_vehicleStruct->m_numExtras++;
                }
            }
        }
        if (assoc[i].flags & 0x110000)
        {
            void* params[] = {(void*)assoc[i].hierarchyID, NULL};
            RwFrameForAllChildren(RwFrameGetParent(m_rwObject), FindFrameFromIdCB, params);
            RwFrame* frame = (RwFrame*)params[1];
            while (frame && !GetFirstObject(frame))
            {
                frame = GetFirstChild(frame);
            }
            if (frame)
            {
                if (assoc[i].flags & 0x10000)
                {
                    atomic = GetFirstObject(frame);
                }
                else
                {
                    v49 = (RpAtomic *)GetFirstObject(frame);
                }
            }
        }
    }
    //v_handling = &mod_HandlingManager.vehicleHandling[LOBYTE(this->dwHandlingIndex)];
    for (size_t i = 0; assoc[i + 1].name; i++)
    {
        if (assoc[i].flags & 0x20208)
        {
            continue;
        }
        void* params[] = {(void*)assoc[i].hierarchyID, NULL};
        RwFrameForAllChildren(RwFrameGetParent(m_rwObject), (int)CClumpModelInfo::FindFrameFromIdCB, params);
        RwFrame* frame = (RwFrame*)params[1];
        if (!frame)
        {
            continue;
        }
        if (assoc[i].flags & 0x10)
        {
            m_numDoors++;
        }
        if (assoc[i].flags & 2)
        {
            v50.param = 0;
            v50.data = 0;
            RwFrameForAllChildren(frame, CollapseFramesCB, frame);
            (*(int (__cdecl **)(int))((char *)&byte_7F090F + 1))((int)&frame[1]->object.type);
            RwFrameForAllObjects(frame[1], CModelCars::Unknown7, &v50);
            if (v50.param && v50.data)
            {
                LODWORD(v50.data->modelling.pos.z) = *(_DWORD *)(v50.param + offsetof(RpAtomic, renderCallback));
                if ( !LODWORD(v50.data->modelling.pos.z) )
                {
                    LODWORD(v50.data->modelling.pos.z) = AtomicDefaultRenderCallBack;
                }
                m_vehicleStruct->field_310 |= 1 << assoc[i].hierarchyID;
            }
        }
        SetVehicleComponentFlags(frame, assoc[i].flags);
        if (assoc[i].flags & 0x10004)
        {
            if (atomic)
            {
                if (assoc[i].flags & 0x10000)
                {
                    RwFrameForAllChildren(frame, CollapseFramesCB, frame);
                    RwFrameUpdateObjects(frame);
                    CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderWheelAtomicCB);
                }
                else
                {
                    RpAtomic* v38 = RpAtomicClone((RpAtomic *)atomic);
                    RpAtomicSetFrame(v38, frame);
                    RpClumpAddAtomic((RpClump*)m_rwObject, v38);
                    CVisibilityPlugins::SetAtomicRenderCallback(v38, CVisibilityPlugins::RenderWheelAtomicCB);
                    if (assoc[i].hierarchyID != 2 && assoc[i].hierarchyID != 5 && (v_handling->modelFlags & 0x20000000))
                    {
                        v40 = RpAtomicClone((RpAtomic *)atomic);
                        RwFrame* v41 = RwFrameCreate();
                        RpAtomicSetFrame(v40, v41);
                        v7F0B00(frame[1], v41);

                        v41->modelling.right = CVector(1.0f, 0.0f, 0.0f).AsRwV3d();
                        v41->modelling.up =    CVector(0.0f, 1.0f, 0.0f).AsRwV3d();
                        v41->modelling.at =    CVector(0.0f, 0.0f, 1.0f).AsRwV3d();
                        v41->modelling.pos = CVector(flt_8A7784 * -0.25, 0.0f, 0.0f).AsRwV3d();
                        v41->modelling.flags |= 0x20003;

                        RpClumpAddAtomic((RpClump*)m_rwObject, v40);
                        CVisibilityPlugins::SetAtomicRenderCallback(v40, CVisibilityPlugins::RenderWheelAtomicCB);
                    }
                }
            }
        }
        else if ((assoc[i].flags & 0x200000) && v49)
        {
            RpAtomic* atomic = RpAtomicClone(v49);
            RpAtomicSetFrame(atomic, frame);
            RpClumpAddAtomic((RpClump*)m_rwObject, atomic);
            CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderWheelAtomicCB);
        }   
    }
}

static int32_t gVehicleTxd;

void CVehicleModelInfo::SetupCommonData()
{
    LoadVehicleColours();
    CLoadingScreen::NewChunkLoaded();
    LoadVehicleUpgrades();
    CLoadingScreen::NewChunkLoaded();
    LoadEnvironmentMaps();
    CLoadingScreen::NewChunkLoaded();
    int32_t gVehicleTxd = CTxdStore::FindTxdSlot("vehicle");
    if ( gVehicleTxd == -1 )
    {
        gVehicleTxd = CTxdStore::AddTxdSlot("vehicle");
    }
    CTxdStore::LoadTxd(gVehicleTxd, "MODELS\\GENERIC\\VEHICLE.TXD");
    CTxdStore::AddRef(gVehicleTxd);
    CTxdStore::SetCurrentTxd(gVehicleTxd);
    CVehicleModelInfo::ms_pLightsTexture = RwTextureRead("vehiclelights128", 0);
    CVehicleModelInfo::ms_pLightsOnTexture = RwTextureRead("vehiclelightson128", 0);
    CLoadingScreen::NewChunkLoaded();
    m_vehicleStruct->m_infoPool = new CPool<CVehicleModelInfo::CVehicleStructure>(50);
    CCarFXRenderer::InitialiseDirtTexture();
}

RwObject* CVehicleModelInfo::SetAtomicFlagCB(RwObject* object, void* data)
{
    CVisibilityPlugins::SetAtomicFlag((RpAtomic*)object, *(uint16_t*)data);
    return object;
}

void CVehicleModelInfo::SetVehicleComponentFlags(RwFrame* frame, uint32_t flags)
{
    tVehicleHandling* v4 = &mod_HandlingManager.vehicleHandling[m_handlingId];
    if (flags & 0x1000)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x80);
    }
    if (flags & 0x400000)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x400);
    }
    if (flags & 0x40000)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x2000);
    }
    if (flags & 0x80)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x10);
    }
    else if (flags & 0x100 && (v4->modelFlags & VEHICLE_HANDLING_MODEL_IS_VAN || !(flags & 0x60)))
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x20);
    }
    else if (flags & 0x20)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x04);
    }
    else if (flags & 0x40)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x08);
    }
    if (flags & 0x8000 && (v4->modelFlags & VEHICLE_HANDLING_MODEL_NOSWING_BOOT || flags & 0x6000))
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x8000);
    }
    if (flags & 0x2000)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x100);
    }
    else if (flags & 0x4000)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x200);
    }
    if (flags & 0x400)
    {
        RwFrameForAllObjects(frame, CVehicleModelInfo::SetAtomicFlagCB, (void*)0x40);
    }
}

void CVehicleModelInfo::LoadEnvironmentMaps()
{
    int16_t particle = CTxdStore::FindTxdSlot("particle");
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(particle);
    if (!gpWhiteTexture)
    {
        gpWhiteTexture = RwTextureRead("white", NULL);
        gpWhiteTexture->name[0] = '@';
        RwTextureSetFilterMode(gpWhiteTexture, rwFILTERLINEAR);
    }
    CTxdStore::PopCurrentTxd();
}

void CVehicleModelInfo::SetCustomCarPlateText(char* text)
{
    if (text)
    {
        strncpy(m_plateText, text, ELEMS_COUNT(m_plateText));
    }
    else
    {
        m_plateText[0] = '\0';
    }
}

const char* CVehicleModelInfo::GetCustomCarPlateText()
{
    return m_plateText ? m_plateText : NULL;
}

static RpAtomic* FindNotNeededMaterialsCB(RpAtomic* atomic, void* data)
{
    RpMaterialList* matList = (RpMaterialList*)data;
    RpMesh* base = (RpMesh*)&atomic->geometry->mesh[1];

    if (!(CVisibilityPlugins::GetAtomicId(atomic) & 0x2000))
    {
        for (size_t i = 0; i < atomic->geometry->mesh->numMeshes; i++)
        {
            for (size_t j = 0; j < matList->numMaterials; j++)
            {
                matList->materials++;
            }
            _rpMaterialListAppendMaterial(matList, base[i].material);
        }
    }
    return atomic;
}

void CVehicleModelInfo::ReduceMaterialsInVehicle()
{
    RpMaterialList matList;
    CMemoryMgr::LockScratchPad();
    CMemoryMgr::InitScratchPad();
    matList.space = 20;
    matList.materials = (RpMaterial**)CMemoryMgr::Malloc(80);
    matList.numMaterials = 0;
    CTimer::GetCurrentTimeInCycles();
    RpClumpForAllAtomics((RpClump*)m_rwObject, FindNotNeededMaterialsCB, &matList);
    for (size_t i = 0; i < m_vehicleStruct->m_numExtras; i++)
    {
        FindNotNeededMaterialsCB(m_vehicleStruct->m_extras[i], &matList);
    }
    CTimer::GetCurrentTimeInCycles();
    CTimer::GetCyclesPerMillisecond();
    _rpMaterialListDeinitialize(&matList);
    CMemoryMgr::ReleaseScratchPad();
    CVisibilityPlugins::ClearClumpForAllAtomicsFlag((RpClump*)m_rwObject, 0x2000);
}

void CVehicleModelInfo::SetCarCustomPlate()
{
	char plateText[9];

	m_plateMaterial = NULL;
	m_plateText[0] = '\0';
	m_plateType = -1;
	strcpy(plateText, "DEFAULT");
	plateText[8] = 0;
	CCustomCarPlateMgr::GeneratePlateText(plateText, 8);
	m_plateMaterial = CCustomCarPlateMgr::SetupClump((RpClump*)GetRwObject(), plateText, m_plateType);
}

void CVehicleModelInfo::StopUsingCommonVehicleTexDicationary()
{
	RwTextureSetFindCallBack(gDefaultTextureFindCB);
	gDefaultTextureFindCB = 0;
}

void CVehicleModelInfo::UseCommonVehicleTexDicationary()
{
	gDefaultTextureFindCB = RwTextureGetFindCallBack();
	RwTextureSetFindCallBack(FindTextureCB);
}

RwTexture* CVehicleModelInfo::FindTextureCB(const RwChar* name)
{
	RwTexture* texture = RwTexDictionaryFindNamedTexture(gVehicleTxd, name);
	if (!texture)
	{
		return texture;
	}
    RwTexDictionary* currentTexDict = RwTexDictionaryGetCurrent();
    texture = RwTexDictionaryFindNamedTexture(currentTexDict, name);
    if (!strncmp(name, "remap", ELEMS_COUNT("remap")))
    {
		if (!texture)
		{
			char texName[32];
			strcpy(texName, name);
			texName[0] = '#';
			return RwTexDictionaryFindNamedTexture(currentTexDict, texName);
		}
		texture->name[0] = '#';
    }
	return texture;
}

uint16_t CVehicleModelInfo::GetNumWheelUpgrades(int wheelNum)
{
	return ms_numWheelUpgrades[wheelNum];
}

void CVehicleModelInfo::SetEditableMaterials(RpClump *clump)
{
	EditableMaterial* data = gVehicleEditableMaterials;
	RpClumpForAllAtomics(clump, CVehicleModelInfo::SetEditableMaterialsCB, &data);
	data->material = 0;
}

RpMaterial* CVehicleModelInfo::SetEditableMaterialsCB(RpMaterial* a1, void* data)
{
	EditableMaterial** a2 = (EditableMaterial**)data;
	EditableMaterial** editMatArray = a2;
	RpMaterial* material = a1;

  result = a1;
  v3 = *(_DWORD *)&a1->color.red;
  v4 = (RpMaterial *)((char *)a1 + 4);
  v5 = v3 & 0xFFFFFF;
	if (ms_pRemapTexture && material->texture && material->texture->name[0] == '#')
	{
		(*editMatArray)->material = material;
		(*editMatArray)->texture = material->texture;
		material->texture = ms_pRemapTexture;
		(*editMatArray)++;
	}
	if (material->texture == ms_pLightsTexture)
	{
		uint8_t lightsOnIdx = -1;
		if (material->color.green == 175 && material->color.blue == 255)
		{
			lightsOnIdx = 0;
		}
		else if (material->color.red == 200 && material->color.green == 255 && material->color.blue == 0)
		{
			lightsOnIdx = 1;
		}
		else if (material->color.green == 255 && material->color.blue == 185)
		{
			lightsOnIdx = 2;
		}
		else if (material->color.green == 60 && material->color.blue == 255)
		{
			lightsOnIdx = 3;
		}
		(*editMatArray)->material = v4;
		(*editMatArray)->texture = v4->texture;
		(*editMatArray)++;
		if (lightsOnIdx != -1)
		{
			if (ms_lightsOn[lightsOnIdx] )
			{
				(*editMatArray)->material = material;
				(*editMatArray)->texture = material->texture;
				(*editMatArray)++;
			(*a2)->material = a1;
			v9 = (EditableMaterial *)((char *)*a2 + 4);
			*a2 = v9;
			v9->material = (RpMaterial *)a1->texture;
			v10 = (EditableMaterial *)((char *)*a2 + 4);
			*a2 = v10;
			v10->material = (RpMaterial *)((char *)a1 + 12);
			v11 = (EditableMaterial *)((char *)*a2 + 4);
			*a2 = v11;
			v11->material = (RpMaterial *)a1->surfaceProps.ambient;
			*a2 = (EditableMaterial *)((char *)*a2 + 4);
			a1->texture = ms_pLightsOnTexture;
			a1->surfaceProps.ambient = gLightSurfProps.ambient;
			a1->surfaceProps.specular = gLightSurfProps.specular;
			a1->surfaceProps.diffuse = gLightSurfProps.diffuse;
			}
		}
	}
	else
	{
		switch ( v5 )
		{
		case 0xFF3C:
		v12 = (unsigned __int8)CVehicleModelInfo::ms_currentCol.red;
		break;
		case 0xAF00FF:
		v12 = (unsigned __int8)CVehicleModelInfo::ms_currentCol.green;
		break;
		case 0xFFFF00:
		v12 = (unsigned __int8)CVehicleModelInfo::ms_currentCol.blue;
		break;
		default:
		if ( v5 != 0xFF00FF )
		return result;
		v12 = (unsigned __int8)CVehicleModelInfo::ms_currentCol.alpha;
		break;
		}
		(*editMatArray)->material = material;
		(*editMatArray)->texture = material->texture;
		(*editMatArray)++;
		material->color.red = CVehicleModelInfo::ms_vehicleColourTable[v12].red;
		material->color.green = CVehicleModelInfo::ms_vehicleColourTable[v12].green;
		material->color.blue = CVehicleModelInfo::ms_vehicleColourTable[v12].blue;
	}
	return result;
}

void CVehicleModelInfo::ResetEditableMaterials(RpClump* clump)
{
	for (size_t i = 1; i < ELEMS_COUNT(gVehicleEditableMaterials); i += 2)
	{
		gVehicleEditableMaterials[i] = gVehicleEditableMaterials[i+1];
	}
}

void CVehicleModelInfo::ChooseVehicleColour(uint8_t& color1, uint8_t& color2, uint8_t& color3, uint8_t& color4, int colorIndex)
{
	if (!m_numColorVariations || CCheat::m_aCheatsActive[33])
	{
		color1 = 0;
		color2 = 0;
		color3 = 0;
		color4 = 0;
	}
	else
	{
		if (CCheat::m_aCheatsActive[32])
		{
			color1 = 126;
			color2 = 126;
			color3 = 126;
			color4 = 126;
		}
		else
		{
			m_lastColorVariation = (colorIndex + m_lastColorVariation) % m_numColorVariations;
			color1 = m_primaryColors[m_lastColorVariation];
			color2 = m_secondaryColors[m_lastColorVariation];
			color3 = m_tertiaryColors[m_lastColorVariation];
			color4 = m_quaternaryColors[m_lastColorVariation];
			if (m_numColorVariations > 1)
			{
				CVehicle* playerVehicle = FindPlayerVehicle(-1, 0);
				if (playerVehicle)
				{
					if ((CVehicleModelInfo *)CModelInfo::GetModelInfo(playerVehicle->GetModelIndex()) == this)
					{
						if (playerVehicle->m_colour1 == color1 && playerVehicle->m_colour2 == color2 && playerVehicle->m_colour3 == color3 && playerVehicle->m_colour4 == color4)
						{
							m_lastColorVariation = (colorIndex + m_lastColorVariation) % m_numColorVariations;
							color1 = m_primaryColors[m_lastColorVariation];
							color2 = m_secondaryColors[m_lastColorVariation];
							color3 = m_tertiaryColors[m_lastColorVariation];
							color4 = m_quaternaryColors[m_lastColorVariation];
						}
					}
				}
			}
		}
    }
}

RwObject* ClearAtomicFlagCB(RwObject* object, void* data)
{
	CVisibilityPlugins::ClearAtomicFlag((RpAtomic*)object, (unsigned short)data);
	return object;
}

RwFrame* CVehicleModelInfo::CollapseFramesCB(RwFrame* frame, void* data)
{
	RwFrameForAllChildren(frame, CollapseFramesCB, data);
	RwFrameForAllObjects(frame, MoveObjectsCB, data);
	RwFrameDestroy(frame);
	return frame;
}

RwObject* CVehicleModelInfo::MoveObjectsCB(RwObject* object, void* data)
{
	RpAtomicSetFrame((RpAtomic*)object, (RwFrame*)data);
	return object;
}

RpMaterial* CVehicleModelInfo::GetMatFXEffectMaterialCB(RpMaterial* material, void* data)
{
	if (!RpMatFXMaterialGetEffects(material) )
	{
		return material;
	}
	data = (void*)RpMatFXMaterialGetEffects(material);
	return 0;
}

RpMaterial* CVehicleModelInfo::SetEnvMapCoeffCB(RpMaterial* material, void* data)
{
	if (RpMatFXMaterialGetEffects(material) == rpMATFXEFFECTENVMAP)
	{
		float coeff = *(float*)&data;
		RpMatFXMaterialSetEnvMapCoefficient(material, coeff / 1000.0f);
	}
	return material;
}

uint16_t CVehicleModelInfo::GetWheelUpgrade(int upgradeNum, int wheelNum)
{
	return CVehicleModelInfo::ms_upgradeWheels[upgradeNum][wheelNum];
}

RpAtomic* CVehicleModelInfo::SetEnvironmentMapAtomicCB(RpAtomic* atomic, void* data)
{
	RpGeometryForAllMaterials(atomic->geometry, SetEnvironmentMapCB, data);
	return atomic;
}

RpMaterial *__cdecl CVehicleModelInfo::SetEnvironmentMapCB(RpMaterial* material, void* data)
{
	if (data == (void *)0xFFFF)
	{
		RpMatFXMaterialSetEffects(material, rpMATFXEFFECTNULL);
	}
	else
	{
		if (RpMatFXMaterialGetEffects(material) == rpMATFXEFFECTENVMAP)
		{
			RpMatFXMaterialSetEnvMapFrame(material, gVehicleEnvMapFrame);
			if (!material->texture)
			{
				RpMaterialSetTexture(material, gpWhiteTexture);
			}
			RpMatFXMaterialSetEnvMapCoefficient(material, 0.25f);
		}
	}
	return material;
}

RpAtomic* CVehicleModelInfo::SetEnvMapCoeffAtomicCB(RpAtomic* atomic, void* data)
{
	RpGeometryForAllMaterials(atomic->geometry, SetEnvMapCoeffCB, data);
	return atomic;
}

void CVehicleModelInfo::SetEnvMapCoeff(float coeff)
{
	RwObject* rwObject = GetRwObject();
	if (rwObject)
	{
		float coeffadj = floor(coeff * 1000.0);
		RpClumpForAllAtomics((RpClump*)rwObject, SetEnvMapCoeffAtomicCB, (void*)*(int*)&coeffadj);
	}
}

RpAtomic* CVehicleModelInfo::SetRenderPipelinesCB(RpAtomic* atomic, void* data)
{
	return atomic;
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Train(RpAtomic* atomic, void* data)
{
	const char* frameNodeName = GetFrameNodeName(RpAtomicGetFrame(atomic));
	RpAtomicCallBackRender renderCB;
	if (strstr(frameNodeName, "_vlo"))
	{
		renderCB = CVisibilityPlugins::RenderVehicleReallyLowDetailCB_BigVehicle;
	}
	else
	{
		bool hasAlphaMaterial = false;
		RpGeometryForAllMaterials(atomic->geometry, (int)CVehicleModelInfo::HasAlphaMaterialCB, (void*)&hasAlphaMaterial);
		if (hasAlphaMaterial)
		{
			renderCB = CVisibilityPlugins::RenderTrainHiDetailAlphaCB;
		}
		else
		{
			renderCB = CVisibilityPlugins::RenderTrainHiDetailCB;
		}
	}
	CVisibilityPlugins::SetAtomicRenderCallback(atomic, renderCB);
	HideDamagedAtomicCB(atomic,NULL);
	return atomic;
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Heli(RpAtomic* atomic, void* data)
{
	const char* frameNodeName = GetFrameNodeName(RpAtomicGetFrame(atomic));
	if (strncmp(frameNodeName, "toprotor", ELEMS_COUNT("toprotor")))
	{
		if (strncmp(frameNodeName, "rearrotor", ELEMS_COUNT("rearrotor")))
		{
			CVisibilityPlugins::SetAtomicRenderCallback(atomic, NULL);
		}
		else
		{
			CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderHeliTailRotorAlphaCB);
		}
	}
	else
	{
		CVisibilityPlugins::SetAtomicRenderCallback(atomic, CVisibilityPlugins::RenderHeliRotorAlphaCB);
	}
	return atomic;
}

CVehicleModelInfo::CVehicleModelInfo()
{
	m_vehicleStruct = NULL;
	m_numColorVariations = 0;
	m_nFlags = 0;
		pThis->vehicleAnimFileIndex = -1;
		memset(pThis->carMods, -1, sizeof(pThis->carMods));
	m_remaps[0] = -1;
	m_remaps[1] = -1;
	m_remaps[2] = -1;
	m_remaps[3] = -1;
}