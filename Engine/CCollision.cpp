#include "StdInc.h"

bool CCollision::ProcessLineTriangle(CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*)
{

}

bool CCollision::ProcessSphereSphere(const CColSphere& first, const CColSphere& second, CColPoint& point, float& depth)
{
    CVector delta = first.GetCenter() - second.GetCenter();
    float fDistSq = delta.MagnitudeSquared();
    float radiiSum = first.GetRadius() + second.GetRadius();
    float touchDistSq = ClampMin(delta.Magnitude() - second.GetRadius(), 0.0f);
    touchDistSq = touchDistSq * touchDistSq;
    if (touchDistSq < depth && fDistSq <= radiiSum * radiiSum)
    {
        delta.Normalise();
        point.pos = first.GetCenter() - delta * touchDistSq;
        point.normal = delta;
        point.surfaceTypeA = first.GetSurfaceTypeA();
        point.lightingA = first.GetLighting();
        point.surfaceTypeB = second.GetSurfaceTypeB();
        point.lightingB = second.GetLighting();
        point.depth = radiiSum - delta.Magnitude();
        return true;
    }
    return false;
}

bool CCollision::ProcessSphereBox(CColSphere const &first, CColBox const &second, CColPoint &point, float &fMaxTouchDistance)
{

}

bool CCollision::ProcessVerticalLine(const CColLine& colLine_, const CMatrix& mat, CColModel& colModel, CColPoint& colPoint, float& depth, bool bSurfaceNotSeeThrough, bool, CStoredCollPoly *pCollPoly)
{
    const CCollisionData *colData = colModel.GetColData();
    if (!colData)
    {
        return false;
    }
    CColLine colLine = colLine_;
    CVector ds = colLine.GetStart() - mat.pos;
    colLine.m_start.x = DotProduct(ds, mat.right);
    colLine.m_start.y = DotProduct(ds, mat.up);
    colLine.m_start.z = DotProduct(ds, mat.at);
    CVector de = colLine.GetEnd() - mat.pos;
    colLine.m_end.x = DotProduct(de, mat.right);
    colLine.m_end.y = DotProduct(de, mat.up);
    colLine.m_end.z = DotProduct(de, mat.at);
    if (!TestLineBox(colLine, colModel))
    {
        return false;
    }
    float currentDepth = depth;
    for (size_t i = 0; i < colData->GetNumSpheres(); i++)
    {
        if(!bSurfaceNotSeeThrough || !g_surfaceInfos.IsSeeThrough(colData->GetSphere(i).surface.material))
        {
            ProcessLineSphere(colLine, colData->m_pSpheres[i], colPoint, currentDepth);
        }
    }
    for(size_t i = 0; i < colData->m_wNumBoxes; i++)
    {
        if(!bSurfaceNotSeeThrough || !g_surfaceInfos.IsSeeThrough(colData->m_pBoxes[i].surface.material))
        {
            ProcessLineBox(colLine, colData->m_pBoxes[i], colPoint, unkFloat);
        }
    }
    CalculateTrianglePlanes(colModel);
    byte_965A20 = 0;
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

bool CCollision::ProcessLineSphere(const CColLine& colLine, const CColSphere& colSphere, CColPoint& colPoint, float& depth)
{
    float r = colSphere.GetRadius();
    CVector l = colLine.GetEnd() - colLine.GetStart();
    // ||l||
    float lSq = l.MagnitudeSquared();
    // c - o
    CVector dco = colSphere.GetCenter() - colLine.GetStart();
    // -(l * dco)
    float b = -DotProduct(l, dco);
    float D = b * b - (dco.MagnitudeSquared() - r * r) * lSq;
    if (D < 0.0f) 
    {   // no intersections
        return false;
    }
    float d = (-b - sqrt(D)) / lSq;
    if (d < 0.0f || d > 1.0f)
    {   // out of bounds of line segment
        return false;
    }
    if (d >= depth)
    {   // passed user-supplied limit
        return false;
    }
    colPoint.pos = colLine.GetStart() + l * d;
    colPoint.normal = colPoint.pos - colSphere.GetCenter();
    colPoint.normal.Normalise();
    colPoint.surfaceTypeB = colSphere.GetSurfaceTypeA();
    colPoint.lightingB = colSphere.GetLighting();
    colPoint.surfaceTypeA = 0;
    colPoint.lightingA = 0;
    depth = d;
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
    CVector midline = first.GetCenter() - second.GetCenter();
    float radiiSum = first.GetRadius() + second.GetRadius();
    return midline.MagnitudeSquared() <= radiiSum * radiiSum;
}

bool CCollision::TestSphereBox(const CSphere& sphere, const CBox& box)
{
    return sphere.center.x + sphere.radius >= box.sup.x
        && sphere.center.x - sphere.radius <= box.inf.x
        && sphere.center.y + sphere.radius >= box.sup.y
        && sphere.center.y - sphere.radius <= box.inf.y
        && sphere.center.z + sphere.radius >= box.sup.z
        && sphere.center.z - sphere.radius <= box.inf.z;  
}

void CCollision::GetBoundingBoxFromTwoSpheres(CColBox* box, CColSphere* s1, CColSphere* s2)
{
    float radius = s1->GetRadius();
    CVector bmin;
    CVector bmax;
    ///
    CColSphere* maxX = (s1->GetCenter().x >= s2->GetCenter().x) ? s1 : s2;
    CColSphere* minX = maxX == s1 ? s2 : s1;
    bmax.x = minX->GetCenter().x - radius;
    bmin.x = maxX->GetCenter().x + radius;
    ///
    CColSphere* maxY = (s1->GetCenter().y >= s2->GetCenter().y) ? s1 : s2;
    CColSphere* minY = maxY == s1 ? s2 : s1;
    bmax.y = minY->GetCenter().y - radius;
    bmin.y = maxY->GetCenter().y + radius;
    ///
    CColSphere* maxZ = (s1->GetCenter().z >= s2->GetCenter().z) ? s1 : s2;
    CColSphere* minZ = maxZ == s1 ? s2 : s1;
    bmax.z = minZ->GetCenter().z - radius;
    bmin.z = maxZ->GetCenter().z + radius;
}

void CCollision::CalculateTrianglePlanes(CColModel* colModel)
{
    if (colModel)
    {
        colModel->CalculateTrianglePlanes();
    }
}

void CCollision::RemoveTrianglePlanes(CColModel* colModel)
{
    if (colModel)
    {
        colModel->RemoveTrianglePlanes();
    }
}

void CCollision::CalculateTrianglePlanes(CCollisionData* colData)
{
    if (!colData->GetNumTriangles())
    {
        return;
    }
    if (colData->m_pTrianglePlanes)
    {
        ms_colModelCache.InsertLink(colData->GetLinkPtr());
    }
    else
    {
        CLink<CCollisionData*>* link = ms_colModelCache.Insert(colData);
        if (!link)
        {
            CLink<CCollisionData*>* last = ms_colModelCache.GetHead().tail;
            last->data->RemoveTrianglePlanes();
            ms_colModelCache.RemoveLink(last);
            link = ms_colModelCache.Insert(colData);
        }
        colData->CalculateTrianglePlanes();
        colData->SetLinkPtr(link);
    }
}

float CCollision::DistToLine(const CVector* origin, const CVector* v1, const CVector* v2)
{
    CVector v1o = CVector(*v1) - CVector(*origin);
    CVector v2o = CVector(*v2) - CVector(*origin);
    float dot = DotProduct(v1o, v2o);
    if (dot <= 0.0f)
    {
        return v2o.Magnitude();
    }
    if (dot < v1o.MagnitudeSquared())
    {
        float d = v2o.Magnitude() - dot / v1o.MagnitudeSquared();
        return d > 0.0f ? d : 0.0f;
    }
    else
    {
        CVector diff = CVector(*v2) - CVector(*v1);
        return diff.Magnitude();
    }
}