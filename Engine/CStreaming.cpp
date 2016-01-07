#include "StdInc.h"

static bool gLoadVehiclesInLoadScene;
static bool gDisableCopBikes;
static uint32_t gGta3ImgDescriptorNum;
static uint32_t gGtaIntImgDescriptorNum;
static RwStream gStreamingRwStream;

void CStreaming::MakeSpaceFor(int size)
{
    while (ms_memoryAvailable - size <= ms_memoryUsed)
    {
        if (!RemoveLeastUsedModel())
        {
            DeleteRwObjectsBehindCamera(ms_memoryAvailable - size);
        }
    }
}

void CStreaming::SetModelIsDeletable(int id)
{
    CStreamingInfo* modelInfo = &ms_aInfoForModel[id];
    modelInfo->m_flags &= ~0x02;
    if (!(modelInfo->m_flags & 0x04))
    {
        if (modelInfo->m_loadStatus == 1)
        {
            if (!modelInfo->InList())
            {
                modelInfo->AddToList(ms_pStartLoadedList);
            }
        }
        else if (!(modelInfo->m_flags & 0x08))
        {
            RemoveModel(id);
        }
    }
}

void CStreaming::SetModelTxdIsDeletable(int id)
{
    SetModelIsDeletable(CModelInfo::GetModelInfo(id)->GetTextureDictionary() + 20000);
}

void CStreaming::SetMissionDoesntRequireModel(int id)
{
    CStreamingInfo* modelInfo = &ms_aInfoForModel[id];
    modelInfo->m_flags &= ~0x04;
    if (!(modelInfo->m_flags & 0x02))
    {
        if (modelInfo->m_loadStatus == 1)
        {
            if (!modelInfo->InList())
            {
                modelInfo->AddToList(ms_pStartLoadedList);
            }
        }
        else if (!(modelInfo->m_flags & 0x08))
        {
            RemoveModel(id);
        }
    }
    if (id < 20000)
    {
        SetMissionDoesntRequireModel(CModelInfo::GetModelInfo(id)->GetTextureDictionary() + 20000);
    }
}

uint32_t CStreaming::AddImageToList(char const* name, bool notPlayerImage)
{
	uint32_t i;
	for (i = 0; i < ELEMS_COUNT(ms_files); i++)
	{
		if (!ms_files[i].name[0])
		{
			break;
		}
	}
	if (i == ELEMS_COUNT(ms_files))
	{
		return 0;
	}
	strcpy(ms_files[i].name, name);
	ms_files[i].streamHandle = CdStreamOpen(name);
	ms_files[i].isNotPlayerImg = notPlayerImage;
	return i;
}

void CStreaming::SetLoadVehiclesInLoadScene(bool loadVehiclesInLoadScene)
{
	gLoadVehiclesInLoadScene = loadVehiclesInLoadScene;
}

void CStreaming::DisableCopBikes(bool disable)
{
	gDisableCopBikes = disable;
}

bool CStreaming::HasSpecialCharLoaded(int id)
{
	return ms_aInfoForModel[id + 290].m_loadStatus == 1;
}

void CStreaming::InitImageList()
{
	for (size_t i = 0; i < ELEMS_COUNT(ms_files); i++)
	{
		ms_files[i].name[0] = '\0';
		ms_files[i].streamHandle = 0;
	}
	gGta3ImgDescriptorNum = AddImageToList("MODELS\\GTA3.IMG", true);
	gGtaIntImgDescriptorNum = AddImageToList("MODELS\\GTA_INT.IMG", true);
}

void CStreaming::RequestModel(uint32_t modelId, uint32_t flags)
{
	if (ms_aInfoForModel[modelId].m_loadStatus == 2)
	{
		if (flags & 0x10 && !(ms_aInfoForModel[modelId].m_flags & 0x10))
		{
			ms_aInfoForModel[modelId].m_flags |= 0x10;
			ms_numPriorityRequests++;
		}
	}
	else if (ms_aInfoForModel[modelId].m_loadStatus != 0)
	{
		ms_aInfoForModel[modelId].m_flags |= flags & 0xEF;
		if (ms_aInfoForModel[modelId].m_loadStatus == 1)
		{
			if (ms_aInfoForModel[modelId].InList())
			{
				ms_aInfoForModel[modelId].RemoveFromList();
				CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelId);
				size_t modelType = modelInfo->GetModelType();
				if (modelId > 19999 ||
				   (modelType != 7 && modelType != 6))
				{
					if (!(ms_aInfoForModel[modelId].m_flags & (2|4)))
					{
						ms_aInfoForModel[modelId].AddToList(ms_pStartLoadedList);
					}
				}
			}
		}
		else if (ms_aInfoForModel[modelId].m_loadStatus != 3 && ms_aInfoForModel[modelId].m_loadStatus != 4)
		{
			ms_aInfoForModel[modelId].m_flags = flags;
			ms_aInfoForModel[modelId].m_loadStatus = 2;
		}
	}
	else
	{
		ms_aInfoForModel[modelId].m_flags |= flags;
		if (modelId > 20000+4999 )
		{
			if (modelId <= 19999)
			{
			v7 = (*(int (**)(void))(*(_DWORD *)CModelInfo::ms_modelInfoPtrs[temp] + 60))();
			if ( v7 != -1 )
			CStreaming::RequestModel(v7 + 25575, 8);
			}
			ms_aInfoForModel[modelId].AddToList(ms_pStartRequestedList);
			ms_numModelsRequested++;
			if (flags & 0x10)
			{
				ms_numPriorityRequests++;
			}
			ms_aInfoForModel[modelId].m_flags = flags;
			ms_aInfoForModel[modelId].m_loadStatus = 2;
		}
		else
		{
			ms_aInfoForModel[modelId].m_loadStatus = 1;
		}
	}
}

bool CStreaming::FinishLoadingLargeFile(char* name, int modelId)
{
	bool success;

	if (ms_aInfoForModel[modelId].m_loadStatus != 4)
	{
		if (modelId < 20000)
		{
			CModelInfo::GetModelInfo(modelId)->RemoveRef();
		}
		return false;
	}
	struct
	{
		char* name;
		size_t size;
	} data;
	data.name = name;
	data.size = ms_aInfoForModel[modelId].m_fileSize << 11;
	RwStream* stream = _rwStreamInitialize(&gStreamingRwStream, false, rwSTREAMMEMORY, rwSTREAMREAD, &data);
	if (modelId >= 20000)
	{
		if (modelId >= 25000)
		{
			success = modelId;
		}
		else
		{
			CTxdStore::AddRef(modelId - 20000);
			success = CTxdStore::FinishLoadTxd(modelId - 20000, stream);
			CTxdStore::RemoveRefWithoutDelete(modelId - 20000);
		}
	}
    else
    {
		CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelId);
		CTxdStore::SetCurrentTxd(modelInfo->GetTextureDictionary());
		success = CFileLoader::FinishLoadClumpFile(stream, modelId);
		if (success)
		{
			success = AddToLoadedVehiclesList(modelId);
		}
		modelInfo->RemoveRef();
		CTxdStore::RemoveRefWithoutDelete(modelInfo->GetTextureDictionary());
		if (modelInfo->GetAnimFileIndex() != -1)
		{
			CAnimManager::RemoveAnimBlockRefWithoutDelete(modelInfo->GetAnimFileIndex());
		}
    }
    RwStreamClose(stream, &data);
	ms_aInfoForModel[modelId].m_loadStatus = 1;
	ms_memoryUsed += ms_aInfoForModel[modelId].m_fileSize << 11;
    if (!success)
    {
		RemoveModel(modelId);
		RequestModel(modelId, CStreaming::ms_aInfoForModel[v3].m_flags);
		return false;
    }
	return true;
}

void CStreaming::RemoveBigBuildings()
{
	CPool<CBuilding>* buildingPool = CPools::GetBuildingPool();
	for (size_t i = 0; i < buildingPool->GetSize(); i++)
	{
		CBuilding* building = buildingPool->Get(i);
		if (!building)
		{
			continue;
		}
		if (building->GetIsBigBuilding())
		{
			CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(building->GetModelIndex());
			if (!modelInfo->bDontCollideWithFlyer)
			{
				building->DeleteRwObject();
				if (modelInfo->GetRefCount() == 0)
				{
					RemoveModel(building->GetModelIndex());
				}
			}
		}
	}
}

CLink<CEntity*>* CStreaming::AddEntity(CEntity* entity)
{
	if (entity->GetType() == ENTITY_TYPE_PED || entity->GetType() == ENTITY_TYPE_VEHICLE)
	{
		return NULL;
	}
	CLink<CEntity*>* result = ms_rwObjectInstances.Insert(entity);
    if (!result)
    {
		for (CLink<CEntity*>* ptr = ms_rwObjectInstances.GetHead().tail; ptr != &ms_rwObjectInstances.GetHead(); ptr = ptr->head->next)
		{

		}
      v3 = (CLinkList *)CStreaming::ms_rwObjectInstances.m_head.tail;
      if ( (CLinkList *)CStreaming::ms_rwObjectInstances.m_head.tail != &CStreaming::ms_rwObjectInstances )
      {
        while ( 1 )
        {
          v4 = *(_DWORD *)(v3->head.data + 28);
          if ( !(BYTE1(v4) & 0x24) )
            break;
          v3 = (CLinkList *)v3->head.next;
          if ( v3 == &CStreaming::ms_rwObjectInstances )
            return (CLinkList *)CLinkList<CEntity__>::Insert(&CStreaming::ms_rwObjectInstances, (CEntity **)&a1);
        }
        (*(void (__thiscall **)(int))(*(_DWORD *)v3->head.data + 0x20))(v3->head.data);
      }
	  result = ms_rwObjectInstances.Insert(entity);
    }
	return result;
}