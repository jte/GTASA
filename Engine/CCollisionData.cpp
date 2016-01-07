#include "StdInc.h"

CCollisionData::CCollisionData()
{
    m_numSpheres = 0;
    m_numBoxes = 0;
    m_numTriangles = 0;
    m_numDisks = 0;
    m_flags &= 0xF8;
    m_spheres = NULL;
    m_boxes = NULL;
    m_disks = NULL;
    m_trianglePoints = NULL;
    m_triangles = NULL;
    m_trianglePlanes = NULL;
    m_numShadowTriangles = 0;
    m_numShadowVertices = 0;
    m_shadowTrianglePoints = NULL;
    m_shadowTriangles = NULL;
}

void CCollisionData::RemoveCollisionVolumes()
{
    if(m_spheres)
    {
        CMemoryMgr::Free(m_spheres);
    }
    if(m_disks)
    {
        CMemoryMgr::Free(m_disks);
    }
    if(m_boxes)
    {
        CMemoryMgr::Free(m_boxes);
    }
    if(m_trianglePoints)
    {
        CMemoryMgr::Free(m_trianglePoints);
    }
    if(m_triangles)
    {
        CMemoryMgr::Free(m_triangles);
    }
    if(m_shadowTriangles)
    {
        CMemoryMgr::Free(m_shadowTriangles);
    }
    if(m_shadowTrianglePoints)
    {
        CMemoryMgr::Free(m_shadowTrianglePoints);
    }
    CCollision::RemoveTrianglePlanes();
    m_numSpheres = 0;
    m_numDisks = 0;
    m_numBoxes = 0;
    m_numTriangles = 0;
    m_spheres = NULL;
    m_boxes = NULL;
    m_disks = NULL;
    m_trianglePoints = NULL;
    m_triangles = NULL;
    m_trianglePlanes = NULL;
    m_shadowTrianglePoints = NULL;
    m_shadowTriangles = NULL;
}

void CCollisionData::CalculateTrianglePlanes()
{
    m_trianglePlanes = CMemoryMgr::Malloc(sizeof(*m_trianglePlanes) * (m_numTriangles + 1));
    if(m_numTriangles)
    {
        for(size_t i = 0; i < m_numTriangles; i++)
        {
            m_trianglePlanes[i].Set(m_trianglePoints, m_triangles[i]);
        }
    }
}

void CCollisionData::GetTrianglePoint(CVector& trianglePoint, int index) const
{
    DecompressVector(trianglePoint, m_trianglePoints[index]);
}

void CCollisionData::RemoveTrianglePlanes()
{
    if(m_trianglePlanes)
    {
        CMemoryMgr::Free(m_trianglePlanes);
    }
    m_trianglePlanes = NULL;
}

CColSphere& CCollisionData::GetSphere(size_t index) const
{
    return m_spheres[index];
}

void CCollisionData::SetSphere(size_t index, const CColSphere& colSphere) 
{
	m_spheres[index] = colSphere;
}

uint16_t CCollisionData::GetNumSpheres() const
{
    return m_numSpheres;
}

uint16_t CCollisionData::GetNumBoxes() const
{
    return m_numBoxes;
}

uint16_t CCollisionData::GetNumTriangles() const
{
    return m_numTriangles;
}

uint8_t CCollisionData::GetNumDisks() const
{
    return m_numDisks;
}