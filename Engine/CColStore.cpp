#include "StdInc.h"

static CVector gCollisionNeededPos;
static bool gCollisionNeeded;
static uint8_t gCollisionRequiredForAreaCode;

void CColStore::AddCollisionNeededAtPosn(const CVector& pos)
{
	gCollisionNeededPos = pos;
	gCollisionNeeded = true;
}

void SetIfCollisionIsRequired(const CVector2D& sector, void* data)
{
	ColDef* colDef = (ColDef*)data;
	if (gCollisionRequiredForAreaCode || !colDef->bInterior)
	{
		if (sector.x >= colDef->area.left && sector.x <= colDef->area.right && 
			sector.y >= colDef->area.bottom && sector.y <= colDef->area.top)
		{
			colDef->bCollisionIsRequired = true;
		}
	}
}

void SetIfCollisionIsRequiredReducedBB(const CVector2D& sector, void* data)
{
	ColDef* colDef = (ColDef*)data;

	if (colDef->area.left - -80.0 <= sector.x
	   && colDef->area.right - 80.0 >= sector.x
	   && colDef->area.bottom - -80.0 <= sector.y
	   && colDef->area.top - 80.0 >= sector.y)
	{
		if (gCollisionRequiredForAreaCode)
		{
			if (!colDef->bInterior)
			{
				return;
			}
		}
		else
		{
			if (colDef->bInterior)
			{
				return;
			}
		}
		colDef->bCollisionIsRequired = true;
	}
}

void CColStore::SetCollisionRequired(const CVector& pos, int areaCode)
{
	if (areaCode == -1)
	{
		CPed* v3 = FindPlayerPed(-1);
		if (v3)
		{
			areaCode = v3->GetAreaCode();
		}
		else
		{
			areaCode = CGame::GetCurrentArea();
		}
	}
	gCollisionRequiredForAreaCode = areaCode;
	CVector2D sector(pos.x, pos.y);
	if (areaCode == CGame::GetCurrentArea())
	{
		ms_pQuadTree->ForAllMatching(sector, SetIfCollisionIsRequired);
	}
	else
	{
		ms_pQuadTree->ForAllMatching(sector, SetIfCollisionIsRequiredReducedBB);
	}
}

void CColStore::RemoveCol(int colNum)
{
	ColDef* colDef = ms_pColPool->Get(colNum);
	for (size_t i = colDef->modelIdStart; i < colDef->modelIdEnd; i++)
	{
		CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(i);
		if (modelInfo && modelInfo->GetColModel())
		{
			CColModel* colModel = modelInfo->GetColModel();
			if (modelInfo->GetDoWeOwnColModel() && colModel->GetColNum())
			{
				colModel->RemoveCollisionVolumes();
			}
		}
	}
}

void CColStore::AddRef(int colNum)
{
	ColDef* colDef = ms_pColPool->Get(colNum);
	if (colDef)
	{
		colDef->refCount++;
	}
}

void CColStore::RemoveRef(int colNum)
{
	ColDef* colDef = ms_pColPool->Get(colNum);
	if (colDef)
	{
		colDef->refCount--;
	}
}

void CColStore::IncludeModelIndex(int colNum, int modelId)
{
	ColDef* colDef = ms_pColPool->Get(colNum);
	if (colDef)
	{
		if (modelId < colDef->modelIdStart)
		{
			colDef->modelIdStart = modelId;
		}
		if (modelId > colDef->modelIdEnd)
		{
			colDef->modelIdEnd = modelId;
		}
	}
}