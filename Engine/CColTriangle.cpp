#include "StdInc.h"

uint16_t CColTriangle::GetVerticeIndex(const uint32_t index) const
{
    assert(index >= 0 && index <= 3);
    return m_verticeIndex[index];
}

uint8_t CColTriangle::GetSurfaceType() const
{
    return m_surfaceType;
}

uint8_t CColTriangle::GetLighting() const
{
    return m_lighting;
}

void CColTriangle::SetVerticeIndex(const uint32_t index, const uint16_t vertice)
{
    assert(index >= 0 && index <= 3);
    m_verticeIndex[index] = vertice;
}

void CColTriangle::SetSurfaceType(const uint8_t surfaceType)
{
    m_surfaceType = surfaceType;
}

void CColTriangle::SetLighting(const uint8_t lighting)
{
    m_lighting = lighting;
}