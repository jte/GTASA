#include "StdInc.h"

CColModel::~CColModel()
{
}

void CColModel::MakeMultipleAlloc()
{
    if(m_flags & 0x02)
    {
        CCollisionData* colData = new CCollisionData;
        colData->Copy(m_colData);
        CMemoryMgr::Free(m_colData);
        m_colData = colData;
        m_flags &= ~0x02;
    }
}

CColModel& CColModel::operator=(const CColModel& other)
{
    m_boundingBox = other.m_boundingBox;
    pad1 = other.pad1;
    pad2 = other.pad2;
    pad3 = other.pad3;
    m_flags = other.m_flags;
    if(m_colData)
    {
        m_colData->Copy(other.m_colData);
    }
}

void* CColModel::operator new(uint32_t size)
{
    return CPools::ms_pColModelPool->New(size);
}

void CColModel::operator delete(void* object)
{
    CPools::ms_pColModelPool->Delete(object);
}

CColModel::CColModel()
{
    pad1 = 0;
    m_colData = NULL;
    m_flags = 0x04;
}

void CColModel::RemoveCollisionVolumes()
{
    if(m_colData)
    {
        if(m_flags & 2)
        {
            CCollision::RemoveTrianglePlanes(m_colData);
        
        }
        else
        {
            m_colData->RemoveCollisionVolumes();
        }
        CMemoryMgr::Free(m_colData);
        m_colData = 0;
    }
}

void CColModel::AllocateData()
{
    m_colData = new CCollisionData;
    m_flags &= ~0x02;
}

void CColModel::CalculateTrianglePlanes()
{
    if(m_colData)
    {
        CCollision::CalculateTrianglePlanes(m_colData);
    }
}

void CColModel::RemoveTrianglePlanes()
{
    if(m_colData)
    {
        CCollision::RemoveTrianglePlanes(m_colData);
    }
}

CColModel::~CColModel()
{
    RemoveCollisionVolumes();
}