#include "StdInc.h"

void CBox::Set(const CVector& min, const CVector& max)
{
    inf = min;
    sup = max;
}

void CBox::Recalc()
{
    if (inf.x > sup.x)
    {
        Exchange(inf.x, sup.x);
    }
    if (inf.y > sup.y)
    {
        Exchange(inf.y, sup.y);
    }
    if (inf.z > sup.z)
    {
        Exchange(inf.z, sup.z);
    }
}