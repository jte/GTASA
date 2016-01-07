#include "StdInc.h"

void CColTrianglePlane::Set(const CompressedVector* points, CColTriangle& triangle)
{
    const CompressedVector V0 = points[triangle.GetVerticeIndex(0)];
    const CompressedVector V1 = points[triangle.GetVerticeIndex(1)];
    const CompressedVector V2 = points[triangle.GetVerticeIndex(2)];
    CVector V[3];
    DecompressVector<128>(V[0], V0);
    DecompressVector<128>(V[1], V1);
    DecompressVector<128>(V[2], V2);
    CVector u = V[1] - V[0];
    CVector v = V[2] - V[0];
    // n = (V[1] - V[0]) x (V[2] - V[0])
    CVector n = CrossProduct(u,v);
    n.Normalise();
    CompressVector<4096>(n, m_n);
    m_d = FloatToFixedPoint<128>(DotProduct(V[0], V[1]));
#pragma message("TODO CColTrianglePlane::Set")
}