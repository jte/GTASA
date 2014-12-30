#pragma once

class CColourSet
{
public:
    //CColourSet(int, int);
	//void Interpolate(CColourSet* first, CColourSet*, float invInterpol, float Interpol, bool basicOnly);

    float m_ambientRed;
    float m_ambientGreen;
    float m_ambientBlue;
    float m_ambientRedObj;
    float m_ambientGreenObj;
    float m_ambientBlueObj;
    float field_18;
    float field_1C;
    float field_20;
    uint16_t m_skyTopRed;
    uint16_t m_skyTopGreen;
    uint16_t m_skyTopBlue;
    uint16_t m_skyBottomRed;
    uint16_t m_skyBottomGreen;
    uint16_t m_skyBottomBlue;
    uint16_t m_sunCoreRed;
    uint16_t m_sunCoreGreen;
    uint16_t m_sunCoreBlue;
    uint16_t m_sunCoronaRed;
    uint16_t m_sunCoronaGreen;
    uint16_t m_sunCoronaBlue;
    float m_sunSize;
    float m_spriteSize;
    float m_spriteBrightness;
    uint16_t m_shadowStrength;
    uint16_t m_lightShadowStrength;
    uint16_t m_poleShadowStrength;
    uint16_t field_4E;
    float m_farClip;
    float m_fogStart;
    float m_lightsOnGroundBrightness;
    uint16_t m_lowCloudsRed;
    uint16_t m_lowCloudsGreen;
    uint16_t m_lowCloudsBlue;
    uint16_t m_fluffyCloudsRed;
    uint16_t m_fluffyCloudsGreen;
    uint16_t m_fluffyCloudsBlue;
    float m_waterRed;
    float m_waterGreen;
    float m_waterBlue;
    float m_waterAlpha;
    float m_postFx1Red;
    float m_postFx1Green;
    float m_postFx1Blue;
    float m_postFx1Alpha;
    float m_postFx2Red;
    float m_postFx2Green;
    float m_postFx2Blue;
    float m_postFx2Alpha;
    float m_cloudAlpha;
    float m_highLightMinIntensity;
    uint16_t field_A0;
    uint16_t field_A2;
    float field_A4;
    float field_A8;
};
