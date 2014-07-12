#include "CPointLights.h"

void CPointLights::AddLight(unsigned char type, CVector origin, CVector direction, float radius, float red, float green, float blue, unsigned char uc8, bool drop_shadow, CEntity *entity)
{
    CVector camPos = camera.GetPos();
    float visible_radius = radius + 15.0;
    CVector dx = origin - camPos;
    if(dx.x >= visible_radius || dx.x <= -visible_radius)
    {
        return;
    }
    if(dx.y >= visible_radius || dx.y <= -visible_radius)
    {
        return;
    }
    if(NumLights >= 32)
    {
        return;
    }
    if(dx.Length() >= visible_radius)
    {
        return;
    }
    size_t id = NumLights;
    aLights[id].ucType = type;
    aLights[id].uc8 = uc8;
    aLights[id].vecOrigin = origin;
    aLights[id].vecDirection = direction;
    aLights[id].fRadius = radius;
    aLights[id].bDropShadow = drop_shadow;
    aLights[id].pEntity = entity;
    float intensity = 1.0;
    if(visible_radius * 0.75 <= dx.Length())
    {
        intensity = 1.0 - (dx.Length() / visible_radius - 0.75) * 4.0;
    }
    aLights[id].fRed = fRed * intensity;
    aLights[id].fGreen = fGreen * intensity;
    aLights[id].fBlue = fBlue * intensity;
    NumLights++;
}

float CPointLights::GenerateLightsAffectingObject(CVector const *pos, float *intensity, CEntity *entity)
{
    if(NumLights <= 0)
    {
        return 0;
    }
    float total_light = 0.0;
    for(size_t i = 0; i < NumLights; i++)
    {
        if(aLights[i].ucType == 3 || aLights[i].ucType == 4)
        {
            continue;
        }
        CVector dx = aLights[i].vecOrigin - pos;
        float radius = aLights[i].fRadius;
        if(dx.x <= -radius)
        {
            continue;
        }
        if(dx.x >= radius)
        {
            continue;
        }
        if(dx.y <= -radius)
        {
            continue;
        }
        if(dx.y >= radius)
        {
            continue;
        }
        if(dx.z <= -radius)
        {
            continue;
        }
        if(dx.z >= radius)
        {
            continue;
        }
        if(dx.Length() >= radius)
        {
            continue;
        }
        if(aLights[i].ucType == 2)
        {
            total_light *= dx.Length() / radius;
        }
        else
        {
            if(intensity)
            {
                *intensity += (1.0 - dx.Length()) * aLights[i].fRed * 0.333;
                *intensity += (1.0 - dx.Length()) * aLights[i].fGreen * 0.333;
                *intensity += (1.0 - dx.Length()) * aLights[i].fBlue * 0.33;
            }
            float brightness = 1.0;
            if(dx.Length() >= 0.5)
            {
                brightness = 2.0 * (1.0 - dx.Length());
            }
            if(dx.Length() == 0.0)
            {
                continue;
            }
            CVector dx_normal = dx.Unit();
            if(aLights[i].ucType == 1 && aLights[i].pEntity != entity)
            {
                // calculate diffuse lighting
                brightness *= max(0.0, 2.0 * (-dx_normal.ScalarProduct(aLights[i].vecDirection) - 0.5));
            }
            if(brightness > 0.0)
            {
                AddAnExtraDirectionalLight(gtaWorld, dx_normal, brightness * aLights[i].fRed, brightness * aLights[i].fGreen, brightness * aLights[i].fBlue);
            }
        }
    }
    return total_light;
}

bool CPointLights::ProcessVerticalLineUsingCache(CVector origin, float *centerZ)
{
    for(size_t i = 0; i < ARRAY_COUNT(aCachedMapReads); i++)
    {
        if(aCachedMapReadReads[i] == origin)
        {
            *centerZ = aCachedMapReadResults[i];
            return 1;
        }
    }
    RwSphere s;
    int dummy;
    if(!CWorld::ProcessVerticalLine(origin, origin.z - 20.0, &s, &dummy, 1, 0, 0, 0, 1, 0))
    {
        return false;
    }
    aCachedMapReadResults[NextCachedValue] = s.center.z;
    aCachedMapReads[NextCachedValue] = origin;
    NextCachedValue = (NextCachedValue + 1) % 32;
    *centerZ = s.center.z;
    return true;
}

void CPointLights::Init()
{
    memset(aCachedMapReadResults, 0, sizeof(aCachedMapReadResults));
    for(size_t i = 0; i < ARRAY_COUNT(aCachedMapReads); i++)
    {
        aCachedMapReads[i] = 0;
    }
}