#include "StdInc.h"

void CModelInfo::Initialise()
{

}

CBaseModelInfo* CModelInfo::GetModelInfo(char const* name, int* index)
{
    size_t hash = CKeyGen::GetUppercaseKey(name);
    size_t pos = 0;
    CBaseModelInfo* info = NULL;
    if (ms_lastPositionSearched < 20000)
    {
        for (pos = 0; pos <= 20000; pos++)
        {
            info = ms_modelInfoPtrs[pos];
            if (info && info->m_uiHashKey == hash)
            {
                break;
            }
        }
    }
    if (ms_lastPositionSearched >= 20000 || pos >= 20000)
    {
        for (pos = 0; pos <= ms_lastPositionSearched; pos++)
        {
            CBaseModelInfo* info = ms_modelInfoPtrs[pos];
            if(info && info->m_uiHashKey == hash)
            {
                break;
            }
        }
        if (pos == ms_lastPositionSearched)
        {
            return NULL;
        }
    }
    if (index)
    {
        *index = pos;
    }
    ms_lastPositionSearched = pos;
    return info;
}

CBaseModelInfo* CModelInfo::GetModelInfoFromHashKey(unsigned int hash, int* index)
{
    CBaseModelInfo* info = NULL;
    for (size_t i = 0; i < 20000; i++)
    {
        info = ms_modelInfoPtrs[i];
        if (info && info->m_uiHashKey == hash)
        {
            if (index)
            {
                *index = i;
            }
            return info;
        }
    }
    return NULL;
}

CBaseModelInfo* CModelInfo::GetModelInfo(char const* name, int low, int high)
{
    size_t hash = CKeyGen::GetUppercaseKey(name);
    for (size_t i = low; i <= high; i++)
    {
        CBaseModelInfo* info = ms_modelInfoPtrs[i];
        if (info && info->m_uiHashKey == hash)
        {
            return info;
        }
    }
    return NULL;
}

CBaseModelInfo* CModelInfo::GetModelInfoUInt16(char const* name, unsigned short* index)
{
    uint32_t idx = 0;
    GetModelInfo(name, &idx);
    if (index)
    {
        *index = idx;
    }
}

bool CModelInfo::IsBikeModel(int index)
{
    return IsVehicleModelType(index) == 9;
}

bool CModelInfo::IsBmxModel(int index)
{
    return IsVehicleModelType(index) == 10;
}

bool CModelInfo::IsBoatModel(int index)
{
    return IsVehicleModelType(index) == 5;
}

bool CModelInfo::IsCarModel(int index)
{
    return IsVehicleModelType(index) == 0;
}

bool CModelInfo::IsFakePlaneModel(int index)
{
    return IsVehicleModelType(index) == 8;
}

bool CModelInfo::IsHeliModel(int index)
{
    return IsVehicleModelType(index) == 3;
}

bool CModelInfo::IsMonsterTruckModel(int index)
{
    return IsVehicleModelType(index) == 1;
}

bool CModelInfo::IsPlaneModel(int index)
{
    return IsVehicleModelType(index) == 4;
}

bool CModelInfo::IsQuadBikeModel(int index)
{
    return IsVehicleModelType(index) == 2;
}

bool CModelInfo::IsTrailerModel(int index)
{
    return IsVehicleModelType(index) == 11;
}

bool CModelInfo::IsTrainModel(int index)
{
    return IsVehicleModelType(index) == 6;
}

uint32_t CModelInfo::IsVehicleModelType(int index)
{
    if(index < 20000)
    {
        CVehicleModelInfo* info = ms_modelInfoPtrs[index];
        if (info && info->GetModelType() == 6)
        {
            return info->VehicleType;
        }
    }
    return -1;
}