#include "StdInc.h"

void CPointLights::AddLight(uint8_t type, CVector position, CVector direction, float range, float red, float green, float blue, uint8_t fogType, bool dropsShadow, CEntity* entityToLight)
{
    CVector camPos = TheCamera.GetPos();
    float visibleRadius = range + 15.0f;
    CVector dx = position - camPos;
    if (dx.x >= visibleRadius || dx.x <= -visibleRadius)
    {
        return;
    }
    if (dx.y >= visibleRadius || dx.y <= -visibleRadius)
    {
        return;
    }
    if (NumLights >= 32)
    {
        return;
    }
    if (dx.Magnitude() >= visibleRadius)
    {
        return;
    }
    size_t id = NumLights;
    aLights[id].m_type = type;
    aLights[id].m_fogType = fogType;
    aLights[id].m_position = position;
    aLights[id].m_direction = direction;
    aLights[id].m_range = range;
    aLights[id].m_generateShadows = dropsShadow;
    aLights[id].m_entityToLight = entityToLight;
    float intensity = 1.0;
    if(visibleRadius * 0.75f <= dx.Magnitude())
    {
        intensity = 1.0 - (dx.Magnitude() / visibleRadius - 0.75f) * 4.0;
    }
    aLights[id].m_red = red * intensity;
    aLights[id].m_green = green * intensity;
    aLights[id].m_blue = blue * intensity;
    NumLights++;
}

float CPointLights::GenerateLightsAffectingObject(const CVector* pos, float* intensity, CEntity* entity)
{
    if (NumLights == 0)
    {
        return 0.0f;
    }
    float totalLight = 0.0f;
    for (size_t i = 0; i < NumLights; i++)
    {
        if (aLights[i].m_type == 3 || aLights[i].m_type == 4)
        {
            continue;
        }
        CVector dx = aLights[i].m_position - pos;
        float radius = aLights[i].m_range;
        if (dx.x <= -radius || dx.x >= radius)
        {
            continue;
        }
        if (dx.y <= -radius || dx.y >= radius)
        { 
            continue;
        }
        if (dx.z <= -radius || dx.z >= radius)
        {
            continue;
        }
        if (dx.Magnitude() >= radius)
        {
            continue;
        }
        if (aLights[i].m_type == 2)
        {
            totalLight *= dx.Magnitude() / radius;
            continue;
        }
        if (intensity)
        {
            *intensity += (1.0 - dx.Magnitude()) * aLights[i].m_red / 3.0f;
            *intensity += (1.0 - dx.Magnitude()) * aLights[i].m_green / 3.0f;
            *intensity += (1.0 - dx.Magnitude()) * aLights[i].m_blue / 3.0f;
        }
        float brightness = 1.0f;
        if (dx.Magnitude() >= 0.5f)
        {
            brightness = 2.0f * (1.0f - dx.Magnitude());
        }
        if (dx.Magnitude() == 0.0f)
        {
            continue;
        }
        CVector normal = dx;
        normal.Normalise();
        if (aLights[i].m_type == 1 && aLights[i].m_entityToLight != entity)
        {
            // calculate diffuse lighting
            brightness *= Max<float>(0.0f, 2.0f * (-DotProduct(normal, aLights[i].m_direction) - 0.5f));
        }
        if (brightness > 0.0f)
        {
            AddAnExtraDirectionalLight(gScene.world, normal, brightness * aLights[i].m_red, brightness * aLights[i].m_green, brightness * aLights[i].m_blue);
        }
    }
    return totalLight;
}

bool CPointLights::ProcessVerticalLineUsingCache(CVector position, float* centerZ)
{
    for (size_t i = 0; i < ELEMS_COUNT(aCachedMapReads); i++)
    {
        if (aCachedMapReads[i] == position)
        {
            *centerZ = aCachedMapReadResults[i];
            return true;
        }
    }
    RwSphere s;
    int dummy;
    if (!CWorld::ProcessVerticalLine(position, position.z - 20.0, &s, &dummy, 1, 0, 0, 0, 1, 0))
    {
        return false;
    }
    aCachedMapReadResults[NextCachedValue] = s.center.z;
    aCachedMapReads[NextCachedValue] = position;
    NextCachedValue = (NextCachedValue + 1) % 32;
    *centerZ = s.center.z;
    return true;
}

void CPointLights::Init()
{
    memset(aCachedMapReadResults, 0, sizeof(aCachedMapReadResults));
    for(size_t i = 0; i < ELEMS_COUNT(aCachedMapReads); i++)
    {
        aCachedMapReads[i] = CVector(0.0f, 0.0f, 0.0f);
    }
}

void CPointLights::RemoveLightsAffectingObject()
{
    RemoveExtraDirectionalLights();
}