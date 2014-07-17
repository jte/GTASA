#include "StdInc.h"

uint32_t CCullZones::FindAttributesForCoors(CVector pos)
{
    uint32_t attribs;
    for(size_t i = 0; i < NumAttributeZones; i++)
    {
        if(CZoneDef::IsPointWithin(pos))
        {
            attribs |= aAttributeZones[i].flags;
        }
    }
    return attribs;
}