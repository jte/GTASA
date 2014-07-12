#include "StdInc.h"

void CReferences::Init()
{
    pEmptyList = aRefs;
    for(size_t i = 0; i < ELEMS_COUNT(m_refs) - 1; i++)
    {
        aRefs[i].pNext = &aRefs[i + 1];
        aRefs[i].pEntity = NULL;
    }
    aRefs[ELEMS_COUNT(aRefs) - 1].next = NULL;
}

size_t CReferences::ListSize(CReference *pStart)
{
    size_t uiSize = 0;
    for(; pStart; uiSize++)
    {
        pStart = pStart->pNext;
    }
    return uiSize;
}

void CReferences::RemoveReferencesToPlayer()
{
    CEntity *pPlayerVeh = FindPlayerVehicle(-1, false);
    if(pPlayerVeh)
    {
        pPlayerVeh->ResolveReferences();
    }
    CEntity *pPlayerPed = FindPlayerPed(-1);
    if(pPlayerPed)
    {
        pPlayerPed->ResolveReferences();
        CPedGroup *pGroup = CPedGroups::ms_groups[pPlayerPed->pPlayerData->playerGroup];
        CPedGroupMembership::SetLeader(pGroup, pPlayerPed);
        pGroup->Process();
    }
}

void CReferences::PruneAllReferencesInWorld()
{
    CPool *pPedPool = CPools::ms_pPedPool;
    for(size_t i = 0; i < pPedPool->GetSize(); i++)
    {
        if(pPedPool[i])
        {
            pPedPool[i]->PruneReferences();
        }
    }
    //TODO
}