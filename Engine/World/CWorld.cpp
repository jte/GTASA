#include "StdInc.h"

void CWorld::FindObjectsInRange(const CVector& vecCenter, float fRadius, bool bIgnoreZ, short *usFoundCount, short usMaxCount, CEntity** pFoundObjectsList, bool bIncludeBuildings, bool bIncludeVehicles, bool bIncludePeds, bool bIncludeObjects, bool bIncludeDummies)
{
    CRect sector;
    sector.left = floor((vecCenter.x - fRadius) * GRID_UNIT + GRID_OFFSET);
    sector.bottom = floor((vecCenter.y - fRadius) * GRID_UNIT + GRID_OFFSET);
    sector.right = floor((fRadius + vecCenter.x) * GRID_UNIT + GRID_OFFSET);
    sector.top = floor((fRadius + vecCenter.y) * GRID_UNIT + GRID_OFFSET);
    if (ms_nCurrentScanCode >= MAX_SCAN_CODE)
    {
        ClearScanCodes();
        ms_nCurrentScanCode = 1;
    }
    else
    {
        ms_nCurrentScanCode++;
    }
    for (; /*sector.bottom < GRID_MAX_UNITS && */sector.bottom <= sector.top; sector.bottom++)
    {
        for (;/* sector.left < GRID_MAX_UNITS && */sector.left <= sector.right; sector.left++)
        {
            CPtrNodeSingle *pSector = ms_aSectors[(sector.left % GRID_CELL_SIZE) + GRID_CELL_SIZE * (sector.bottom % GRID_CELL_SIZE)];
            CPtrNodeDouble *pRepeatSector = ms_aRepeatSectors[Clamp(0, sector.left, MAX_GRID_UNITS-1) + MAX_GRID_UNITS * Clamp(0, sector.bottom, MAX_GRID_UNITS-1)];
            if (bIncludeBuildings)
            {
                FindObjectsInRangeSectorList(pSector, vecCenter, fRadius, bIgnoreZ, usFoundCount, usMaxCount, pFoundObjectsList);
            }
            if (bIncludeVehicles)
            {
                FindObjectsInRangeSectorList(pRepeatSector, vecCenter, fRadius, bIgnoreZ, usFoundCount, usMaxCount, pFoundObjectsList);
            }
            if (bIncludePeds)
            {
                FindObjectsInRangeSectorList(pRepeatSector->next, vecCenter, fRadius, bIgnoreZ, usFoundCount, usMaxCount, pFoundObjectsList);
            }
            if (bIncludeObjects)
            {
                FindObjectsInRangeSectorList(pRepeatSector->prev, vecCenter, fRadius, bIgnoreZ, usFoundCount, usMaxCount, pFoundObjectsList);
            }
            if (bIncludeDummies)
            {
                FindObjectsInRangeSectorList(pSector->next, vecCenter, fRadius, bIgnoreZ, usFoundCount, usMaxCount, pFoundObjectsList);
            }
        }
    }
}

void CWorld::FindObjectsInRangeSectorList(CPtrList &pNode, CVector const &vecCenter, float fRadius, bool bIgnoreZ, short *usFoundCount, short usMaxCount, CEntity **pFoundObjectsList)
{
    while (pNode && pNode->ptr)
    {
        CObject *pObject = pNode->ptr;
        if (pObject->m_nScanCode != CWorld::ms_nCurrentScanCode)
        {
            pObject->m_nScanCode = CWorld::ms_nCurrentScanCode;
            CVector vecObjectPos = pObject->GetPos();
            float fDistance;
            CVector vecDiffPos = vecObjectPos - vecCenter;
            if (bIgnoreZ)
            {
                fDistance = vecDiffPos.Magnitude2DSquared();
            }
            else
            {
                fDistance = vecDiffPos.MagnitudeSquared();
            }
            if (fDistance < fRadius * fRadius)
            {
                if (*usFoundCount < usMaxCount)
                {
                    if (pFoundObjectsList)
                    {
                        pFoundObjectsList[*usFoundCount] = (CEntity*)pObject;
                    }
                    (*usFoundCount)++;
                }
            }
        }
        pNode = pNode->prev;
    }
}

void CWorld::ResetLineTestOptions()
{
    bIncludeDeadPeds = false;
    bIncludeCarTyres = false;
    bIncludeBikers = false;
    pIgnoreEntity = NULL;
    fWeaponSpreadRate = 0.0;
}

bool CWorld::CameraToIgnoreThisObject(CEntity *pEntity)
{
    if (CGarages::IsModelIndexADoor(pEntity->GetModelIndex()))
    {
        return false;
    }
    switch(pEntity->pObjectInfo->bCameraAvoidObject)
    {
    case 1:
        return gCurCamColVars >= 10;
    case 2:
        return false;
    case 3:
        return gCurCamColVars < 10;
    }
    return true;
}

void CWorld::Add(CEntity *pEntity)
{
    RwObject *pObject = pEntity->m_pRwObject;
    if(pObject)
    {
        pEntity->UpdateRwMatrix();
    }
    pEntity->UpdateRwFrame();
    pEntity->Add();
    if(pEntity->GetEntityType() != ENTITY_TYPE_BUILDING && pEntity->GetEntityType() != ENTITY_TYPE_DUMMY)
    {
        if(!pEntity->bIsStatic && !pEntity->bIsStaticWaitingForCollision)
        {
            pEntity->AddToMovingList();
        }
    }
}

void CWorld::Remove(CEntity *pEntity)
{
    pEntity->Remove();
    if(pEntity->GetEntityType() >= ENTITY_TYPE_VEHICLE && pEntity->GetEntityType() <= ENTITY_TYPE_OBJECT)
    {
        pEntity->RemoveFromMovingList();
    }
}

void CWorld::ClearForRestart()
{
    if (CCutsceneMgr::ms_cutsceneLoadStatus == 2 )
    {
        CCutsceneMgr::DeleteCutsceneData();
    }
    CProjectileInfo::RemoveAllProjectiles();
    CObject::DeleteAllTempObjects();
    CObject::DeleteAllMissionObjects();
    CPopulation::ConvertAllObjectsToDummyObjects();

    CPtrList* pNode = CWorld::ms_aRepeatSectors->pNext;
    while (pNode)
    {
        Remove(pNode->pData);
        delete pNode->pData;
        pNode = pNode->pNext;
    }
    CPickups::ReInit();
    CPools::CheckPoolsEmpty();
}

CPed* FindPlayerPed(int number)
{
	return CWorld::Players[number < 0 ? CWorld::PlayerInFocus : number].pPed;
}