#include "StdInc.h"

bool CCollision::TestSphereBox(const CSphere& sphere, const CBox& box)
{
  return sphere->center.x + sphere->radius >= box->max.x
      && sphere->center.x - sphere->radius <= box->min.x
      && sphere->center.y + sphere->radius >= box->max.y
      && sphere->center.y - sphere->radius <= box->min.y
      && sphere->center.z + sphere->radius >= box->max.z
      && sphere->center.z - sphere->radius <= box->min.z;  
}

bool CCollision::ProcessLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*)
{

}

bool CCollision::ProcessSphereSphere(CColSphere const &first, CColSphere const &second, CColPoint &point, float &fMaxTouchDistance)
{
    CVector vecDelta = first.vecCenter - second.vecCenter;
    float fDistSq = vecDelta.LengthSquared();
    float fRadiiSum = first.fRadius + second.fRadius;
    float fTouchDist = vecDelta.Length() - second.fRadius;
    if(fTouchDist < 0.0)
    {
        fTouchDist = 0.0;
    }
    fTouchDist *= fTouchDist;
    if(fTouchDist < fMaxTouchDistance && fDistSq <= fRadiiSum * fRadiiSum)
    {
        vecCenterDistance.Normalise();
        point.Position = first.vecCenter - vecDelta * fTouchDist;
        point.Normal = vecDelta;
        point.ucSurfaceTypeA = first.ucSurfaceType;
        point.ucSurfaceTypeB = second.ucSurfaceType;
        point.ucLightingA = first.ucLighting;
        point.ucLightingB = second.ucLighting;
        point.fDepth = (first.fRadius + second.fRadius) - vecDelta.Length();
        return true;
    }
    return false;
}

bool CCollision::ProcessSphereBox(CColSphere const &first, CColBox const &second, CColPoint &point, float &fMaxTouchDistance)
{

}

bool CCollision::ProcessVerticalLine(CColLine const &colLine, CMatrix const &mat, CColModel &colModel, CColPoint &colPoint, float &argFloat, bool, bool, CStoredCollPoly *pCollPoly)
{
    CCollisionData *colData = colModel.pColData;
    if(!colData)
    {
        return false;
    }
    CVector ds = colLine.vecStart - mat.matrix.pos;
    colLine.vecStart.x = DotProduct(ds, mat.matrix.right);
    colLine.vecStart.y = DotProduct(ds, mat.matrix.top);
    colLine.vecStart.z = DotProduct(ds, mat.matrix.at);
    CVector de = colLine.vecEnd - mat.matrix.pos;
    colLine.vecEnd.x = DotProduct(de, mat.matrix.right);
    colLine.vecEnd.y = DotProduct(de, mat.matrix.top);
    colLine.vecEnd.z = DotProduct(de, mat.matrix.at);
    if(!TestLineBox_DW(colLine, colModel)) // OR TestLineBox()
    {
        return false;
    }
    float unkFloat = argFloat;
    // process spheres; sizeof(ColSphere) = 0x14
    for(size_t i = 0; i < colData->m_wNumSpheres; i++)
    {
        if(!bSurfaceNotSeeThrough || !g_surfaceInfos.IsSeeThrough(colData->m_pSpheres[i].surface.material))
        {
            ProcessLineSphere(colLine, colData->m_pSpheres[i], colPoint, unkFloat);
        }
    }
    // process boxes; sizeof(ColBox) = 0x1C
    for(size_t i = 0; i < colData->m_wNumBoxes; i++)
    {
        if(!bSurfaceNotSeeThrough || !g_surfaceInfos.IsSeeThrough(colData->m_pBoxes[i].surface.material))
        {
            ProcessLineBox(colLine, colData->m_pBoxes[i], colPoint, unkFloat);
        }
    }
    CalculateTrianglePlanes(colModel);
    byte_965A20 = 0;
    // process triangles; sizeof(ColTrianglePlane) = 0xA
    for(size_t i = 0; i < colData->m_wNumTriangles; i++)
    {
        if(!bSurfaceNotSeeThrough || !g_surfaceInfos.IsSeeThrough(colData->m_pTriangles[i].material))
        {
            ProcessLineTriangle(colLine, m_pTrianglePoints, &m_pTriangles[i], m_pTrianglePlanes[i], colPoint, unkFloat, &unk_9659FC);
        }
    }
    if(unkFloat < argFloat)
    {
        colPoint.Position *= mat;
        colPoint.Normal = Multiply3x3(mat, colPoint.Normal);
        if(gStoredCollPoly.bInitialized)
        {
            pCollPoly = gStoredCollPoly;
            pCollPoly->V0 *= mat;
            pCollPoly->V1 *= mat;
            pCollPoly->V2 *= mat;
        }
        argFloat = unkFloat;
        return true;
    }
    else
    {
        return false;
    }
}

bool CCollision::ProcessLineSphere(const CColLine& colLine, const CColSphere& colSphere, CColPoint& colPoint, float& limit)
{
    float r = colSphere.sphere.radius;
    CVector l = colLine.vecEnd - colLine.vecStart;
    // ||l||
    float lSq = l.LengthSquared();
    // c - o
    CVector dco = colSphere.sphere.center - colLine.vecStart;
    // -(l * dco)
    float b = -DotProduct(l, dco);
    float D = b * b - (dco.LengthSquared() - r * r) * lSq;
    if(D < 0.0f) 
    {   // no intersections
        return false;
    }
    float d = (-b - sqrt(D)) / lSq;
    if(d < 0.0f || d > 1.0f)
    {   // out of bounds of line segment
        return false;
    }
    if(d >= limit)
    {   // passed user-supplied limit
        return false;
    }
    colPoint.Position = colLine.vecStart + l * d;
    colPoint.Normal = colPoint.Position - colSphere.sphere.center;
    colPoint.Normal.Normalise();
    colPoint.ucSurfaceTypeB = colSphere.surface.material;
    colPoint.ucLightingB = colSphere.surface.brightness;
    colPoint.ucSurfaceTypeA = 0;
    colPoint.ucLightingA = 0;
    limit = d;
    ms_iProcessLineNumCrossings += 2;
    return true;
}

bool ProcessDiscCollision(CColPoint &p1, const CMatrix& mat, const CColDisk& disk, CColPoint& p2, bool& b1, float& f1, CColPoint& p3)
{
    CColPoint p1Copy = p1; // transformed by "mat"
    p1Copy.Position *= mat;
    p1Copy.Normal = Multiply3x3(p1Copy.Normal, mat);

    float dot1 = DotProduct(disk.centerLine, p1Copy.Normal);
    CVector dx = p1Copy.Position - disk.base;
    float dot2 = DotProduct(disk.centerLine, dx);
    if(fabs(dot1) >= 0.77f || fabs(dot2) > disk.length)
    {
        if(disk.field_11 < 17 && p1.depth > p2.depth)
        {
            p2 = p1;
            p2.depth = p1.depth;
            p2.surfaceType = 60; // WHEELBASE
            return true;
        }
    }
    else
    {
        float d = sqrt(disk.radius * disk.radius - dx.y * dx.y - dx.x * dx.x) + p1Copy.z;
        if(d >= f1)
        {
            b1 = true;
            f1 = d;
            p3 = p1;
            p3->depth = p1->depth;
        }
    }
    return 0;
}

void CCollision::Update()
{
}

void CCollision::SortOutCollisionAfterLoad()
{
    CColStore::LoadCollision(TheCamera.GetPos(), false);
    CStreaming::LoadAllRequestedModels(false);
}

bool CCollision::TestSphereSphere(const CColSphere& first, const CColSphere& second)
{
    CVector dr = first.GetCenter() - second.GetCenter();
    float touchDistance = first.GetRadius() + second.GetRadius();
    return dr.LengthSquared() <= touchDistance * touchDistance;
}