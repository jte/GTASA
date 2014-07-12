#include "StdInc.h"

CLinkList *CStreaming::AddEntity(CEntity *pEntity)
{
    if(bIsTypeVehicle || bIsTypePed)
    {
        return NULL;
    }
    CLinkList *pLink = ms_rwObjectInstances.Insert(&pEntity);
    if(!pLink)
    {
        CLinkList *pTail = ms_rwObjectInstances.headList_Tail;
        if(pTail != &ms_rwObjectInstances)
        {
            while(!(bIsStatic && bIsInSafePosition))
            {
                pTail = pTail->pPrev;
                if(pTail == &ms_rwObjectInstances)
                {
                    return ms_rwObjectInstances.Insert(&pEntity);
                }
            }
            pTail->DeleteRwObject();
        }
        ms_rwObjectInstances.Insert(&pEntity);
    }
    return pLink;
}

bool CStreaming::FinishLoadingLargeFile(int a3, int index)
{
    if(ms_aInfoForModel[index].bLoadStatus != 4)
    {
        if(index < 20000)
        {
            ms_modelInfoPtrs[index]->RemoveRef();
        }
        return false;
    }
    RwStream stream;
    _rwStreamInitialize(&stream, 0, 3, 1, &data);
    if(index >= 20000)
    {
        if(index < 25000)
        {
            CTxdStore::AddRef(index - 20000);
        }
    }
    else
    {
        CBaseModelInfo *pModelInfo = CModelInfo::ms_modelInfoPtrs;
        CTxdStore::SetCurrentTxd(pModelInfo->usTextureDictionary);
        if(CFileLoader::FinishLoadClumpFile(stream, index))
        {
            CStreaming::AddToLoadedVehiclesList(index);
        }
        pModelInfo->RemoveRef();
        CTxdStore::RemoveRefWithoutDelete(pModelInfo->usTextureDictionary);
        int32_t iAnimFileIndex = pModelInfo->GetAnimFileIndex();
        if(iAnimFileIndex != -1)
        {
            CAnimManager::RemoveAnimBlockRefWithoutDelete(iAnimFileIndex);
        }
    }
    RwStreamClose(stream, &data);
    ms_aInfoForModel[index] = 1;
    ms_memoryUsed += ms_aInfoForModel[index].fileSize;
    if(bSuccess)
    {
        UpdateMemoryUsed();
    }
    else
    {
        RemoveModel(index);
        RequestModel(index, ms_aInfoForModel[index].ucFlags);
    }
    return bSuccess;
}
