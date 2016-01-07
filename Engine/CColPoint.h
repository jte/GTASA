#pragma once

#include "SurfaceInfos_c.h"
#include "Maths\CVector.h"

struct CColLighting
{
    uint32_t day : 4;
    uint32_t night : 4;
};

class CColPoint
{
public:
    CVector pos;
    float pad1;
    CVector normal;
    float pad2;
    EColSurface surfaceTypeA;
    uint8_t pieceTypeA;
    CColLighting lightingA;
    EColSurface surfaceTypeB;
    uint8_t pieceTypeB;
    CColLighting lightingB;
    uint8_t pad3[2];
    float depth;
private:
};