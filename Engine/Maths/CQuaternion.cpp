#include "StdInc.h"

const float DotProduct(const CQuaternion& first, const CQuaternion& second)
{
    return DotProduct(first.pos, second.pos) + first.w * second.w;
}

RwMatrixTag *CQuaternion::Get(RwMatrixTag *pMat) const
{
    float sqx = pos.x + pos.x;
    float sqy = pos.y + pos.y;
    float sqz = pos.z + pos.z;
    float sqxx = sqx * pos.x;
    float sqyx = sqy * pos.x;
    float sqzx = sqz * pos.x;
    float sqyy = sqy * pos.y;
    float sqzy = sqz * pos.y;
    float sqzz = sqz * pos.z;
    float sqxr = sqx * w;
    float sqyr = sqy * w;
    float sqzr = sqz * w;
    pMat->right.x = 1.0f - (sqzz + sqyy);
    pMat->up.x = sqyx - sqzr;
    pMat->at.x = sqyr + sqzx;
    pMat->right.y = sqzr + sqyx;
    pMat->up.y = 1.0f - (sqzz + sqxx);
    pMat->at.y = sqzy - sqxr;
    pMat->right.z = sqzx - sqyr;
    pMat->up.z = sqxr + sqzy;
    pMat->at.z = 1.0f - (sqyy + sqxx);
    return pMat;
}

float CQuaternion::Slerp(CQuaternion const &q1, CQuaternion const &q2, float lx, float ly, float lz)
{
    float slerp = 0.0;
    if(lx == 0.0f)
    {
        pos = q2.pos;
        w = q2.w;
    }
    else
    {
        float a0 = 0.0f;
        float a1 = 0.0f;
        float a2 = 0.0f;
        if(lx <= static_cast<float>(M_PI_2))
        {
            a1 = sin((1.0f - lz) * lx) * ly;
            a2 = sin(lx * lz) * ly;
        }
        else
        {
            a0 = static_cast<float>(M_PI) - lx;
            a1 = sin((1.0f - lz) * a0) * ly;
            a2 = -(sin(a0 * lz) * ly);
        }
        pos.x = a1 * q1.pos.x + a2 * q2.pos.x;
        pos.y = a2 * q2.pos.y + a1 * q1.pos.y;
        pos.z = a2 * q2.pos.z + a1 * q1.pos.z;
        w = a2 * q2.w + a1 * q1.w;
    }
    return slerp;
}

void CQuaternion::Set(RwV3d *vector, float angle)
{
    float halfang = angle * 0.5f;
    float sinhfa = sin(halfang);
    pos.x = sinhfa * vector->x;
    pos.y = sinhfa * vector->y;
    pos.z = sinhfa * vector->z;
    w = cos(halfang);
}

CQuaternion CQuaternion::operator-()
{
    CQuaternion quat;
    quat.pos.x = -quat.pos.x;
    quat.pos.y = -quat.pos.y;
    quat.pos.z = -quat.pos.z;
    quat.w = -quat.w;
    return quat;
}