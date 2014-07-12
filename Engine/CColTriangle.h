#pragma once

class CColTriangle
{
public:
    uint16_t GetVerticeIndex(const uint32_t index) const;
    uint8_t GetSurfaceType() const;
    uint8_t GetLighting() const;
    void SetVerticeIndex(const uint32_t index, const uint16_t vertice);
    void SetSurfaceType(const uint8_t surfaceType);
    void SetLighting(const uint8_t lighting);
protected:
private:
    uint16_t m_verticeIndex[3];
    uint8_t m_surfaceType;
    uint8_t m_lighting;
};