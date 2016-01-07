#pragma once

typedef int CRoadsignAttrFlags;

#define PLUGIN_2DEFFECT(object, var) \
    (RWPLUGINOFFSET(Plugin2dEffectDef, object, g2dEffectPluginOffset)->var)

#define rwID_2DEFFECT  (0xF8)

extern RwInt32 g2dEffectPluginOffset;

struct Effect2dLightA
{

};

struct Effect2dLightB
{
    RwRGBA color;
    float coronaFarClip;
    float pointLightRange;
    float coronaSize;
    float shadowSize;
    uint8_t coronaShowMode;
    uint8_t coronaEnableReflection;
    uint8_t coronaFlareType;
    uint8_t shadowColorMultiplier;
    uint8_t flags16b;
    RwTexture *pCoronaTexture;
    RwTexture *pShadowTexture;
    uint8_t shadowZDistance;
};
/*
00000000 Effect2dDescriptorLightB struc ; (sizeof=0x40)
00000000 header          Effect2dDescriptor ?
00000010 color           RwRGBA ?
00000014 coronaFarClip   dd ?
00000018 pointLightRange dd ?
0000001C coronaSize      dd ?
00000020 shadowSize      dd ?
00000024 flags16b        dw ?
00000026 coronaShowMode  db ?
00000027 coronaEnableReflection db ?
00000028 coronaFlareType db ?
00000029 shadowColorMultiplier db ?
0000002A shadowZDistance db ?
0000002B field_2B        db ?
0000002C field_2C        db ?
0000002D field_2D        db ?
0000002E field_2E        db ?
0000002F field_2F        db ?
00000030 pCoronaTexture  dd ?
00000034 pShadowTexture  dd ?
00000038 field_38        dd ?
0000003C field_3C        dd ?
00000040 Effect2dDescriptorLightB ends
*/

struct Effect2dCoverPoint
{
    float dirxy[2];
    uint32_t type;
};

struct Effect2dEscalator
{
    RwV3d bottom;
    RwV3d top;
    RwV3d end;
    uint8_t direction;
};

struct Effect2dStreetSign
{
    RwV2d size;
    RwV3d rotation;
    uint16_t flags;
    char *text;
    uint32_t pad0;
};

struct Effect2dEnExMarker
{
    float rotation;
    float x_radius;
    float y_radius;
    float pos[3];
    float angle;
    uint16_t interior;
    uint8_t flag1;
    uint8_t flag2;
    char name[8];
    uint32_t sky_color;
};

struct Plugin2dEffectDescriptor
{
    RwV3d position;
    uint32_t effect_type; /* actually 1 byte padded with 3 bytes */
    union
    {
        // 0
        Effect2dLightA lightA;
        Effect2dLightB lightB;
        // 1
        char particle_effect[24];
        // 6
        Effect2dEnExMarker marker;
        // 7
        Effect2dStreetSign street_sign;
        // 8
        uint32_t slotmachine_index;
        // 9
        Effect2dCoverPoint cover_point;
        // 10
        Effect2dEscalator escalator;
    };
};

struct Plugin2dEffect
{
    uint32_t count;
    Plugin2dEffectDescriptor entries[];
};

struct Plugin2dEffectDef
{
    Plugin2dEffect* data;
};


class C2dEffect
{
public:
	static bool PluginAttach();
	size_t Roadsign_GetNumLettersFromFlags(CRoadsignAttrFlags flags);
	size_t Roadsign_GetNumLinesFromFlags(CRoadsignAttrFlags flags);
	size_t Roadsign_GetPaletteIDFromFlags(CRoadsignAttrFlags flags);
	void Shutdown();
protected:
private:
};