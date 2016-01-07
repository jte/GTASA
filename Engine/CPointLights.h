#pragma once

struct PointLight_t
{
    CVector m_position;
    CVector m_direction;
    float m_range;
    float m_red;
    float m_green;
    float m_blue;
    CEntity* m_entityToLight;
    uint8_t m_type;
    uint8_t m_fogType;
    uint8_t m_generateShadows;
};

class CPointLights
{
public:
    static void AddLight(uint8_t type, CVector position, CVector direction, float range, float red, float green, float blue, uint8_t fogType, bool dropsShadow, CEntity* entityToLight);
    static float GenerateLightsAffectingObject(const CVector* pos, float* intensity, CEntity* entity);
    static void Init();
    static bool ProcessVerticalLineUsingCache(CVector position, float* centerZ);
    static void RemoveLightsAffectingObject();
    /*
	* GetLightMultiplier(CVector const*)
	* RenderFogEffect()
    */
private:
    static PointLight_t aLights[32];
	static uint32_t NumLights;
    static uint32_t NextCachedValue;
	static float aCachedMapReadResults[32];
	static CVector aCachedMapReads[32];
};