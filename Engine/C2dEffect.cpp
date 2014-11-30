#include "StdInc.h"

#define rwID_2DEFFECT  (0xF8)

#define PLUGIN_2DEFFECT(object, var) \
    (RWPLUGINOFFSET(Plugin2dEffectDef, object, g2dEffectPluginOffset)->var)

static RwInt32 g2dEffectPluginOffset = -1;

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

static void* Effect2dPluginConstructor(void* object, RwInt32 offset, RwInt32 size)
{
    PLUGIN_2DEFFECT(object, data) = NULL;
    return object;
}

static void* Effect2dPluginDestructor(void* object, RwInt32 offset, RwInt32 size)
{

}

static void* Effect2dPluginCopyConstructor(void* dst, const void* src, RwInt32 offset, RwInt32 size)
{
    PLUGIN_2DEFFECT(dst, data) = NULL;
    return dst;
}

static RwInt32 Effect2dPluginGetSize(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    return -1;
}

static RwStream* Effect2dPluginWrite(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    return stream;
}

bool C2dEffect::PluginAttach()
{
    g2dEffectPluginOffset = RpGeometryRegisterPlugin(sizeof(Plugin2dEffectDef),
                                                     MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_2DEFFECT),
                                                     Effect2dPluginConstructor,
                                                     Effect2dPluginDestructor,
                                                     Effect2dPluginCopyConstructor);
    RpGeometryRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_2DEFFECT),
                                   Effect2dPluginRead,
                                   Effect2dPluginWrite,
                                   Effect2dPluginGetSize);
    return g2dEffectPluginOffset != -1;
}

size_t C2dEffect::Roadsign_GetPaletteIDFromFlags(CRoadsignAttrFlags flags)
{
    switch (flags & 3)
    {
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        default: return 4;
    }
}

size_t C2dEffect::Roadsign_GetNumLettersFromFlags(CRoadsignAttrFlags flags)
{
    switch ((flags >> 2) & 3)
    {
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        default: return 16;
    }
}

size_t C2dEffect::Roadsign_GetNumLinesFromFlags(CRoadsignAttrFlags flags)
{
    switch ((flags >> 4) & 3)
    {
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        default: return 0;
    }
}

static RwStream* Effect2dPluginRead(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
  Effect2dDescriptor *v5; // eax@4
  int v6; // edx@4
  Plugin2dEffect *entries_; // ecx@4
  Plugin2dEffect *entries; // eax@4
  Effect2dDescriptor *v9; // ebp@5
  char *v10; // edi@5
  char *v11; // esi@5
  char v12; // al@6
  int v13; // eax@7
  int v14; // ecx@8
  int v15; // edx@8
  char v16; // al@8
  int v17; // ecx@8
  __int16 v18; // ax@8
  __int16 v19; // ax@8
  int v20; // eax@10
  int v21; // ecx@10
  int v22; // edx@10
  int v23; // eax@10
  int *v24; // eax@14
  char v25; // cl@15
  int v26; // edx@19
  int v27; // eax@19
  int v28; // ecx@19
  int v29; // edx@19
  void *v30; // eax@19
  int v31; // edx@23
  int v32; // ecx@23
  int v33; // eax@23
  int v34; // edx@23
  int v35; // ecx@23
  int v36; // edx@23
  char v37; // dl@23
  char v38; // al@23
  int v39; // eax@26
  char v40; // cl@26
  int v41; // eax@33
  int v42; // ecx@33
  int v43; // edx@33
  int v44; // eax@33
  int v45; // ecx@33
  int v46; // edx@33
  int v47; // eax@33
  int v48; // ecx@33
  int v49; // edx@33
  int v50; // ecx@36
  int v51; // eax@36
  int v52; // edx@36
  int v53; // ecx@36
  int v54; // edx@36
  int v55; // eax@36
  int v56; // ecx@36
  int v57; // eax@36
  char v58; // cf@38
  char *v60; // [sp-30h] [bp-1ACh]@8
  char *v61; // [sp-2Ch] [bp-1A8h]@8
  int offset; // [sp+0h] [bp-17Ch]@6
  unsigned int buffer; // [sp+4h] [bp-178h]@3
  unsigned int v64; // [sp+8h] [bp-174h]@5
  int v65; // [sp+Ch] [bp-170h]@4
  int *v66; // [sp+10h] [bp-16Ch]@5
  int *v67; // [sp+14h] [bp-168h]@5
  int v68; // [sp+18h] [bp-164h]@30
  Plugin2dEffect *entries__; // [sp+1Ch] [bp-160h]@4
  int v70; // [sp+20h] [bp-15Ch]@6
  int v86; // [sp+70h] [bp-10Ch]@8
  int v87; // [sp+74h] [bp-108h]@23
  int v88; // [sp+78h] [bp-104h]@23
  int v89; // [sp+7Ch] [bp-100h]@23
  int v90; // [sp+80h] [bp-FCh]@23
  int v91; // [sp+84h] [bp-F8h]@23
  int v92; // [sp+88h] [bp-F4h]@23
  int v93; // [sp+8Ch] [bp-F0h]@23
  __int16 v94; // [sp+90h] [bp-ECh]@23
  char v95; // [sp+92h] [bp-EAh]@23
  char v96; // [sp+93h] [bp-E9h]@23
  char v97; // [sp+94h] [bp-E8h]@23
  char v98; // [sp+9Ch] [bp-E0h]@21
  char v99; // [sp+9Dh] [bp-DFh]@21
  char v100; // [sp+9Eh] [bp-DEh]@23
  int v101; // [sp+A0h] [bp-DCh]@26
  int v102; // [sp+A4h] [bp-D8h]@26
  char v103; // [sp+A8h] [bp-D4h]@26
  int v104; // [sp+ACh] [bp-D0h]@33
  int v105; // [sp+B0h] [bp-CCh]@33
  int v106; // [sp+B4h] [bp-C8h]@33
  int v107; // [sp+B8h] [bp-C4h]@33
  int v108; // [sp+BCh] [bp-C0h]@33
  int v109; // [sp+C0h] [bp-BCh]@33
  int v110; // [sp+C4h] [bp-B8h]@33
  int v111; // [sp+C8h] [bp-B4h]@33
  int v112; // [sp+CCh] [bp-B0h]@33
  char v113; // [sp+D0h] [bp-ACh]@33
  int v114; // [sp+D4h] [bp-A8h]@36
  int v115; // [sp+D8h] [bp-A4h]@36
  int v116; // [sp+DCh] [bp-A0h]@36
  int v117; // [sp+E0h] [bp-9Ch]@36
  int v118; // [sp+E4h] [bp-98h]@36
  int v119; // [sp+E8h] [bp-94h]@36
  int v120; // [sp+ECh] [bp-90h]@36
  int v121; // [sp+F0h] [bp-8Ch]@36
  int v122; // [sp+F4h] [bp-88h]@36
  int v123; // [sp+F8h] [bp-84h]@36
  char v124; // [sp+FCh] [bp-80h]@36
  char v125; // [sp+104h] [bp-78h]@36
  char v126; // [sp+108h] [bp-74h]@36
  char v127; // [sp+10Ah] [bp-72h]@36
  int v128; // [sp+10Ch] [bp-70h]@5
  int v129; // [sp+124h] [bp-58h]@19
  int v130; // [sp+128h] [bp-54h]@19
  int v131; // [sp+12Ch] [bp-50h]@19
  int v132; // [sp+130h] [bp-4Ch]@19
  int v133; // [sp+134h] [bp-48h]@19
  __int16 v134; // [sp+138h] [bp-44h]@19
  int v135; // [sp+13Ah] [bp-42h]@19

    if (index == -1)
    {
        index = CTxdStore::FindTxdSlot("particle");
    }
    uint32_t entries_count;
    RwStreamRead(stream, &entries_count, sizeof(entries_count));             // number of 2dfx entries
    if (!entries_count)
    {
        return stream;
    }
    entries = (Plugin2dEffect *)CMemoryMgr::Malloc(entries_count * sizeof(Plugin2dEffectDescriptor) + sizeof(Plugin2dEffect));
    entries->count = entries_count;

    /**
     * There could be an undefined condition where entry is of unknown type but we just allocate array according to entries_count
     * thus the unknown entry will be filled with garbage
     */
    for (size_t i = 0; i < entries_count; i++)
    {
        RwV3d pos;
        RwStreamRead(stream, &pos, sizeof(pos)); // 12 bytes
        uint32_t entry_type;
        RwStreamRead(stream, &entry_type, sizeof(entry_type)); // 4 bytes
        uint32_t entry_size;
        RwStreamRead(stream, &entry_size, sizeof(entry_size)); // 4 bytes

        entries->entries[i].effect_type = entry_type;
        entries->entries[i].position = pos;
        
        switch (entry_type)
        {
            case 0: // Light
                if (entry_size == 80) // Entry Type 0 - Light v1; 80 bytes
                {
                    RwStreamRead(stream, &v71, offset);
                    v9->field_10 = v71;
                    v9->field_14 = v72;
                    v9->field_18 = v73;
                    v9->field_1C = v74;
                    v9->field_20 = v75;
                    *(_WORD *)&v9->field_24 = (v84 << 8) + v80;
                    v9->field_26 = v76;
                    v9->field_27 = v77;
                    v9->field_28 = v78;
                    v9->field_29 = v79;
                    v9->field_2A = v15;
                    v9->field_2B = v85;
                    v9->field_2C = v86;
                    v9->field_2D = HIBYTE(v86);
                    LOBYTE(v15) = v83;
                    CTxdStore::PushCurrentTxd();
                    CTxdStore::SetCurrentTxd(index);
                    v9->field_30 = RwReadTexture(&name, 0);
                    v9->field_34 = RwReadTexture(v82, 0);
                    CTxdStore::PopCurrentTxd();
                }
                else if (entry_size == 76) // Entry type 0 - Light v2; 76 bytes
                {
                    Effect2dLightB &light = entries->entries[i].lightB;
                    RwStreamRead(stream, &light.color, sizeof(light.color)); // 4 bytes
                    RwStreamRead(stream, &light.coronaFarClip, sizeof(light.coronaFarClip)); // 4 bytes
                    RwStreamRead(stream, &light.pointLightRange, sizeof(light.pointLightRange)); // 4 bytes
                    RwStreamRead(stream, &light.coronaSize, sizeof(light.coronaSize)); // 4 bytes
                    RwStreamRead(stream, &light.shadowSize, sizeof(light.shadowSize)); // 4 bytes
                    RwStreamRead(stream, &light.coronaShowMode, sizeof(light.coronaShowMode)); // 1 byte
                    RwStreamRead(stream, &light.coronaEnableReflection, sizeof(light.coronaEnableReflection)); // 1 byte
                    RwStreamRead(stream, &light.coronaFlareType, sizeof(light.coronaFlareType));
                    RwStreamRead(stream, &light.shadowColorMultiplier, sizeof(light.shadowColorMultiplier));
                    uint8_t flagsl;
                    RwStreamRead(stream, &flagsl, sizeof(flagsl));
                    CTxdStore::PushCurrentTxd();
                    CTxdStore::SetCurrentTxd(index);
                    char corona_tex[24];
                    RwStreamRead(stream, corona_tex, sizeof(corona_tex)); // 24 bytes
                    light.pCoronaTexture = RwTextureRead(corona_tex, NULL);
                    char shadow_tex[24];
                    RwStreamRead(stream, shadow_tex, sizeof(shadow_tex)); // 24 bytes
                    light.pShadowTexture = RwTextureRead(shadow_tex, NULL);
                    CTxdStore::PopCurrentTxd();
                    RwStreamRead(stream, &light.shadowZDistance, sizeof(light.shadowZDistance)); // 1 byte
                    uint8_t flagsh;
                    RwStreamRead(stream, &flagsh, sizeof(flagsh)); // 1 byte
                    light.flags16b = (flagsh << 8) + flagsl;
                    uint32_t dummy;
                    RwStreamRead(stream, &dummy, sizeof(dummy)); // 4 bytes
                }
            break;
            case 1: // Particle effects
                if (entry_size == 24)
                {
                    RwStreamRead(stream, &entries->entries[i].particle_effect, entry_size);
                }
            break;
            case 3: // PED actors
                if (entry_size == 56)
                {
                    RwStreamRead(stream, &v114, offset);
                    v9->field_14 = v116;
                    v9->field_10 = v115;
                    v9->field_20 = v119;
                    v9->field_18 = v117;
                    v9->field_1C = v118;
                    *(_DWORD *)&v9->field_2C = v122;
                    *(_DWORD *)&v9->field_24 = v120;
                    *(_DWORD *)&v9->field_28 = v121;
                    BYTE1(v9->field_34) = v125;
                    *(_DWORD *)v11 = v123;
                    LOBYTE(v9->field_34) = v114;
                    BYTE2(v9->field_34) = v126;
                    BYTE3(v9->field_34) = v127;
                    strncpy(v10, &v124, 8u);
                }
            break;
            case 7: // Entry Type 7 - Street sign; 88 bytes
                if (entry_size == 88)
                {
                    Effect2dStreetSign &sign = entries->entries[i].street_sign;
                    RwStreamRead(stream, &sign.size, sizeof(sign.size)); // 8 bytes
                    RwStreamRead(stream, &sign.rotation, sizeof(sign.rotation)); // 12 bytes
                    uint32_t flags;
                    RwStreamRead(stream, &flags, sizeof(flags)); // 4 bytes
                    sign.flags = flags;
                    sign.text = (char*)CMemoryMgr::Malloc(4*16);
                    char text[4][16];
                    RwStreamRead(stream, text, sizeof(text)); // 64 bytes
                    memcpy(sign.text, text, sizeof(text)); 
                    sign.pad0 = 0;
                }
            break;
            case 6: // Entry type 6 - EnEx markers; 44 or 40 bytes
                v13 = offset;
                v98 = 0;
                v99 = 24;
                if ( offset == 44 || offset == 40 )
                {
                RwStreamRead(stream, &v87, offset);
                v31 = v87;
                v32 = v89;
                v9->field_14 = v88;
                v33 = v91;
                v9->field_10 = v31;
                v34 = v90;
                v9->field_20 = v33;
                LOWORD(v33) = v94;
                v9->field_18 = v32;
                v35 = v92;
                v9->field_1C = v34;
                v36 = v93;
                *(_WORD *)&v9->field_2C = v33;
                *(_DWORD *)&v9->field_24 = v35;
                LOBYTE(v35) = v95;
                *(_DWORD *)&v9->field_28 = v36;
                LOBYTE(v36) = v96;
                v9->field_2E = v35;
                v9->field_2F = v36;
                strncpy(v11, &v97, 8u);
                v37 = v99;
                v38 = v100;
                *v10 = v98;
                BYTE1(v9->field_38) = v37;
                BYTE2(v9->field_38) = v38;
                }
            break;
            case 8: // Entry type 8 - Slotmachine-wheels
                if (entry_size == 4)
                {
                    RwStreamRead(stream, &entries->entries[i].slotmachine_index, entry_size);
                }
            break;
            case 9: // Entry type 9 - conver points; 12 bytes
                if (entry_size == 12)
                {
                    RwStreamRead(stream, &entries->entries[i].cover_point, entry_size);
                }
            break;
            case 10: // Entry type 10 - Escalators
                if (entry_size == 40)
                {
                    Effect2dEscalator &escalator = entries->entries[i].escalator;
                    RwStreamRead(stream, &escalator.bottom, sizeof(escalator.bottom)); // 12 bytes
                    RwStreamRead(stream, &escalator.top, sizeof(escalator.top)); // 12 bytes
                    RwStreamRead(stream, &escalator.end, sizeof(escalator.end)); // 12 bytes
                    uint32_t direction;
                    RwStreamRead(stream, &direction, sizeof(direction)); // 4 bytes
                    escalator.direction = direction ? 1 : 0;
                }
            break;
            default:
                RwStreamSkip(stream, entry_size);
            break;
        }
    }
    PLUGIN_2DEFFECT(object, data) = entries;
    return stream;
}