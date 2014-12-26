#include "StdInc.h"

#define rwID_ENVMAP     (0xFC)
#define rwID_ENVMAPATM  (0xF4)
#define rwID_SPECMAP (0xF6)

static D3DLIGHT9 gCarEnvMapLight;

#define PLUGIN_ENVMAP(object, var) \
    (RWPLUGINOFFSET(EnvMapPluginDef, object, ms_envMapPluginOffset)->var)
#define PLUGIN_ENVMAP_STREAM(object, offset) \
    (RWPLUGINOFFSET(EnvMapPluginDef, object, offset))
#define PLUGIN_ENVMAPATM(object, var) \
    (RWPLUGINOFFSET(EnvMapAtmPluginDef, object, ms_envMapAtmPluginOffset)->var)
#define PLUGIN_SPECMAP(object, var) \
    (RWPLUGINOFFSET(SpecMapPluginDef, object, ms_specularMapPluginOffset)->var)
#define PLUGIN_SPECMAP_STREAM(object, offset) \
    (RWPLUGINOFFSET(SpecMapPluginDef, object, offset))

struct EnvMapPluginDef
{
    CustomEnvMapPipeMaterialData* data;
};

struct EnvMapAtmPluginDef
{
    CustomEnvMapPipeAtomicData* data;
};

struct SpecMapPluginDef
{
    CustomSpecMapPipeMaterialData* data;
};

RxPipeline* CCustomCarEnvMapPipeline::CreateCustomD3D9ObjPipe()
{
    RxPipeline* pipe = RxPipelineCreate();
    if(!pipe)
    {
        return NULL;
    }
    RxLockedPipe* lock = RxPipelineLock(pipe);
    if(!lock)
    {
        RxPipelineDestroy(pipe);
        return NULL;
    }
    RxNodeDefinition* def = RxNodeDefinitionGetD3D9AtomicAllInOne();
    RxPipeline* fragment = RxLockedPipeAddFragment(lock, 0, def, 0);
    if(!RxLockedPipeUnlock(fragment))
    {
        RxPipelineDestroy(pipe);
        return NULL;
    }
    RxPipelineNode* node = RxPipelineFindNodeByName(pipe, def->name, NULL, NULL);
    RxD3D9AllInOneSetInstanceCallBack(node, RxD3D9AllInOneGetInstanceCallBack(node));
    RxD3D9AllInOneSetReinstanceCallBack(node, CustomPipeReInstanceCB);
    RxD3D9AllInOneSetRenderCallBack(node, CustomPipeRenderCB);
    pipe->pluginId = CUSTOM_CAR_ENV_MAP_PIPELINE;
    pipe->pluginData = CUSTOM_CAR_ENV_MAP_PIPELINE;
    return pipe;
}

RwBool CCustomCarEnvMapPipeline::CustomPipeReInstanceCB(void* object, RwResEntry *resEntry, RxD3D9AllInOneInstanceCallBack instanceCallback)
{
    if(!instanceCallback)
    {
        return true;
    }
    return instanceCallback(object, (RxD3D9ResEntryHeader*)(resEntry + 1), true);
}

static void D3D9RenderNotLit(RxD3D9ResEntryHeader* resEntryHeader, RxD3D9InstanceData* instanceData)
{
    RwD3D9SetPixelShader(NULL);
    RwD3D9SetVertexShader((IDirect3DVertexShader9*)instanceData->vertexShader);
    if(resEntryHeader->indexBuffer)
    {
        RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType, instanceData->baseIndex, 0, instanceData->numVertices, instanceData->startIndex, instanceData->numPrimitives);
    }
    else
    {
        RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType, instanceData->baseIndex, instanceData->numPrimitives);
    }
}

static void D3D9RenderPreLit(RxD3D9ResEntryHeader* resEntryHeader, RxD3D9InstanceData* instanceData, RwUInt8 flags, RwTexture* texture)
{
    if(flags & rxGEOMETRY_TEXTURED2 | rxGEOMETRY_TEXTURED)
    {
        RwD3D9SetTexture(texture, 0);
        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    else
    {
        RwD3D9SetTexture(NULL, 0);
        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    RwD3D9SetPixelShader(NULL);
    RwD3D9SetVertexShader((IDirect3DVertexShader9*)instanceData->vertexShader);
    if(resEntryHeader->indexBuffer)
    {
        RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType, instanceData->baseIndex, 0, instanceData->numVertices, instanceData->startIndex, instanceData->numPrimitives);
    }
    else
    {
        RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType, instanceData->baseIndex, instanceData->numPrimitives);
    }
}

static RwSurfaceProperties defaultSurfaceProperties = {1.0f, 0.0f, 0.0f};
static RwRGBA defaultSurfaceColor = {255, 255, 255, 255};

static void D3D9RestoreSurfaceProperties()
{
    RwD3D9SetSurfaceProperties(&defaultSurfaceProperties, &defaultSurfaceColor, 32);
}

static D3DMATERIAL9 material;

signed int __cdecl D3D9SetRenderMaterialProperties(RwSurfaceProperties* surfaceProperties, RwRGBA* color, char flags, float specular, float specularSharpness)
{_rwD3D9LightsOpen
    material.Specular.r = specular;
    material.Specular.g = specular;
    material.Specular.b = specular;
    material.Power = specularSharpness;
    if(flags & rxGEOMETRY_MODULATE && color->red != 255 && color->green != 255 && color->blue != 255 && color->alpha != 255)
    {
        material.Diffuse.r = color->red * (surfaceProperties->diffuse / 255.0f);
        material.Diffuse.g = color->green * (surfaceProperties->diffuse / 255.0f);
        material.Diffuse.b = color->blue * (surfaceProperties->diffuse / 255.0f);
        material.Diffuse.a = color->alpha / 255.0f;
        if ( flags & rxGEOMETRY_PRELIT )
        {
            RwD3D9SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(color->alpha, color->red, color->green, color->blue));
            RwD3D9SetRenderState(D3DRS_COLORVERTEX, 1);
            RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, 1);
            RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, 0);
            material.Ambient.r = 0;
            material.Ambient.g = 0;
            material.Ambient.b = 0;
            material.Emissive.r = color->red * AmbientSaturated.red * (surfaceProperties->ambient / 255.0f);
            material.Emissive.g = color->green * AmbientSaturated.green * (surfaceProperties->ambient / 255.0f);
            material.Emissive.b = color->blue * AmbientSaturated.blue * (surfaceProperties->ambient / 255.0f);
        }
        else
        {
            RwD3D9SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255,255,255,255));
            RwD3D9SetRenderState(D3DRS_COLORVERTEX, 0);
            RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, 0);
            RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, 0);
            material.Ambient.r = color->red * AmbientSaturated.red * (surfaceProperties->ambient / 255.0f);
            material.Ambient.g = color->green * AmbientSaturated.green * (surfaceProperties->ambient / 255.0f);
            material.Ambient.b = color->blue * AmbientSaturated.blue * (surfaceProperties->ambient / 255.0f);
            material.Emissive.r = 0;
            material.Emissive.g = 0;
            material.Emissive.b = 0;
        }
    }
    else
    {
        material.Diffuse.r = surfaceProperties->diffuse;
        material.Diffuse.g = surfaceProperties->diffuse;
        material.Diffuse.b = surfaceProperties->diffuse;
        material.Diffuse.a = 1.0f;
        RwD3D9SetRenderState(D3DRS_AMBIENT, -1);
        RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, 0);
        if(flags & rxGEOMETRY_PRELIT)
        {
            RwD3D9SetRenderState(D3DRS_COLORVERTEX, 1);
            RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, 1);
        }
        else
        {
            RwD3D9SetRenderState(D3DRS_COLORVERTEX, 0);
            RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, 0);
            material.Emissive.r = 0;
            material.Emissive.g = 0;
            material.Emissive.b = 0;
        }
        if(surfaceProperties->ambient == 1.0f)
        {
            material.Ambient.r = AmbientSaturated.red;
            material.Ambient.g = AmbientSaturated.green;
            material.Ambient.b = AmbientSaturated.blue;
        }
        else
        {
            material.Ambient.r = AmbientSaturated.red * surfaceProperties->ambient;
            material.Ambient.g = AmbientSaturated.green * surfaceProperties->ambient;
            material.Ambient.b = AmbientSaturated.blue * surfaceProperties->ambient;
        }
    }
    RwD3D9SetMaterial(&material);
    return 1;
}

static void D3D9GetTransScaleVector(CustomEnvMapPipeMaterialData* data, RpAtomic* atomic, RwV2d* transScale)
{
    RpClump* clump = atomic->clump;
    RwFrame* frame = (RwFrame*)(clump ? clump->object.parent : atomic->object.object.parent);
    const RwV3d& pos = RwFrameGetLTM(frame)->pos;
    float transScaleX = FixedPointToFloat<8>(data->transScaleX) * 50.0f;
    float transScaleY = FixedPointToFloat<8>(data->transScaleY) * 50.0f;

    transScale->x = -((pos.x - static_cast<RwInt32>(pos.x / transScaleX) * transScaleX) * 1.0f / transScaleX);
    transScale->y = -((pos.y - static_cast<RwInt32>(pos.y / transScaleY) * transScaleY) * 1.0f / transScaleY);
}

void CCustomCarEnvMapPipeline::CustomPipeRenderCB(RwResEntry* repEntry, void* object, RwUInt8 type, RwUInt32 flags)
{
    RpAtomic* atomic = (RpAtomic*)atomic;
    RwUInt32 geometryFlags = atomic->geometry->flags;
    RwBool atomicFlag = CVisibilityPlugins::GetAtomicId(atomic) & 0x6000;
    _rwD3D9EnableClippingIfNeeded(object, type);

    RwBool lighting = 0;
    RwD3D9GetRenderState(D3DRS_LIGHTING, &lighting);
    RwBool isLit;
	RwBool renderDark = RpLightGetFlags(pDirect) & rpLIGHTLIGHTATOMICS && CVisibilityPlugins::GetAtomicId(atomic) & 0x4000;	// R* added this in 1.01
    if(lighting || flags & rxGEOMETRY_PRELIT)
    {
        isLit = 0;
    }
    else
    {
        isLit = 1;
        RwD3D9SetTexture(NULL, 0);
        RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0xFF, 0, 0, 0));
        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    }

    RxD3D9ResEntryHeader* resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

    if(resEntryHeader->indexBuffer)
    {
        RwD3D9SetIndices((IDirect3DIndexBuffer9*)resEntryHeader->indexBuffer);
    }
    _rwD3D9SetStreams(resEntryHeader->vertexStream, resEntryHeader->useOffsets);
    RwD3D9SetVertexDeclaration((IDirect3DVertexDeclaration9*)resEntryHeader->vertexDeclaration);
    RxD3D9InstanceData* instancedData = (RxD3D9InstanceData*)(resEntryHeader + 1);
    RwInt32 numMeshes = resEntryHeader->numMeshes;
    while(numMeshes--)
    {
        RpMaterial* material = instancedData->material;
        RwUInt32* materialFlags = (RwUInt32*)&(material->surfaceProps.specular);
        bool applyEnvMap = true;
        if(!(*materialFlags & 1) || atomicFlag)
        {
            applyEnvMap = false;
        }
        bool applyAlphaBlending = true;
        if(!(*materialFlags & 2) || atomicFlag || !(geometryFlags & rpGEOMETRYLOCKTEXCOORDS4))
        {
            applyAlphaBlending = false;
        }
        RwBool applySpecMap = lighting == true;
        float specularLighting = 0.0f;
        float specularPower = 0.0f;
        if(g_fx.GetFxQuality() < 2 && (!(*materialFlags & 8) || atomicFlag))
        {
            applySpecMap = false;
        }
        else
        {
            applySpecMap = *materialFlags & 4;
        }
        if(applySpecMap && !renderDark)	// R* added renderDark in 1.01)
        {
            CustomSpecMapPipeMaterialData* specMapData = PLUGIN_SPECMAP(material, data);
            RwD3D9SetLight(1, &gCarEnvMapLight);
            RwD3D9EnableLight(1, 1);
            specularLighting = 2.0f * m_EnvMapLightingMult * 1.85f * specMapData->specularity;
            ClampMax(specularLighting, 1.0f);
            specularPower = specMapData->specularity * 100.0f;
            RwD3D9SetRenderState(D3DRS_SPECULARENABLE, 1);
            RwD3D9SetRenderState(D3DRS_LOCALVIEWER, 0);
            RwD3D9SetRenderState(D3DRS_SPECULARMATERIALSOURCE, 0);
        }
        else
        {
            RwD3D9SetRenderState(D3DRS_SPECULARENABLE, 0);
        }
        RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
        if(applyEnvMap && !renderDark)	// R* added renderDark in 1.01
        {
            CustomEnvMapPipeMaterialData* envMapData = PLUGIN_ENVMAP(material, data);
            D3DMATRIX EnvMapTransform;
            RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)1);
			// As the matrix is allocated on stack (unlike in R*'s code), the matrix should be zeroed first
			// so untouched fields don't contain garbage
			memset(&EnvMapTransform, 0, sizeof(EnvMapTransform));
            EnvMapTransform._11 = FixedPointToFloat<8>(envMapData->scaleX);
            EnvMapTransform._22 = FixedPointToFloat<8>(envMapData->scaleY);
            EnvMapTransform._33 = 1.0f;
            EnvMapTransform._44 = 1.0f;
            RwV2d transScale;
            D3D9GetTransScaleVector(envMapData, atomic, &transScale);
            EnvMapTransform._31 = transScale.x;
            EnvMapTransform._32 = transScale.y;
            RwD3D9SetTransform(D3DTS_TEXTURE1, &EnvMapTransform);
            RwD3D9SetTexture(envMapData->texture, 1);
            uint8_t texShine = FixedPointToFloat<255>(envMapData->shininess) * m_EnvMapLightingMult * 1.85f * 254.0f;
            ClampMax<uint8_t>(texShine, 255);
            RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(texShine, texShine, texShine));
            RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
            RwD3D9SetTextureStageState(1, D3DTSS_COLORARG0, D3DTA_CURRENT);
            RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
            RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
            RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
            RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL | 1);
            RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED | D3DTTFF_COUNT1 | D3DTTFF_COUNT2);
            RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }
        if(applyAlphaBlending && !renderDark)	// R* added renderDark in 1.01)
        {
            CustomEnvMapPipeAtomicData* envMapAtmData = AllocEnvMapPipeAtomicData(atomic);
            CustomEnvMapPipeMaterialData* envMapData = PLUGIN_ENVMAP(material, data);
            D3DMATRIX AlphaBlendTransform;
            RwV2d scale;
            D3D9GetVectorForAlphaBlendingTransform(atomic, envMapAtmData, envMapData, &scale);
			// As the matrix is allocated on stack (unlike in R*'s code), the matrix should be zeroed first
			// so untouched fields don't contain garbage
			memset(&AlphaBlendTransform, 0, sizeof(AlphaBlendTransform));
            AlphaBlendTransform._11 = 1.0f;
            AlphaBlendTransform._22 = 1.0f;
            AlphaBlendTransform._33 = 1.0f;
            AlphaBlendTransform._44 = 1.0f;
            AlphaBlendTransform._31 = scale.x;
            AlphaBlendTransform._32 = scale.y;
            RwD3D9SetTransform(D3DTS_TEXTURE1, &AlphaBlendTransform);
            RwD3D9SetTexture(envMapData->texture, 1);
            uint8_t alpha = m_EnvMapLightingMult * 1.85f * 24.0f;
            ClampMax<uint8_t>(alpha, 255);
            RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, alpha));
            RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDFACTORALPHA);
            RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
            RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
            RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
            RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
            RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
        }
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)(instancedData->vertexAlpha || material->color.alpha != 255));
        if(isLit)
        {
            D3D9RenderNotLit(resEntryHeader, instancedData);
        }
        else
        {
            if(lighting)
            {
                RwRGBA matColor = material->color;
                if((matColor.red == 0xAF && matColor.blue == 0xFF)  ||
                    (matColor.green == 0xFF && matColor.blue == 0xB9)||
                    (matColor.green == 0xFF && matColor.blue == 0x3C)||
                    (matColor.green == 0x3C && matColor.blue == 0xFF)||
                    (matColor.green == 0xAF && matColor.blue == 0xFF)||
                    (matColor.red == 0xC8 && matColor.green == 0xFF && matColor.blue == 0x00)||
                    (matColor.red == 0xFF && matColor.green == 0x00 && matColor.blue == 0xFF)||
                    (matColor.red == 0xFF && matColor.green == 0xFF && matColor.blue == 0x00))
                {
                    matColor.red = 0;
                    matColor.green = 0;
                    matColor.blue = 0;
                }
                D3D9SetRenderMaterialProperties(&material->surfaceProps, &matColor, flags, specularLighting, specularPower);
            }
            D3D9RenderPreLit(resEntryHeader, instancedData, flags, instancedData->material->texture);
        }
        if(applySpecMap)
        {
            RwD3D9SetRenderState(D3DRS_SPECULARENABLE, 0);
            RwD3D9EnableLight(1, 0);
        }
        instancedData++;
    }
    D3D9RestoreSurfaceProperties();
    RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}

void CCustomCarEnvMapPipeline::PreRenderUpdate()
{
    memset(&gCarEnvMapLight, 0, sizeof(gCarEnvMapLight));
    RwFrame* lightFrame = (RwFrame*)pDirect->object.object.parent;
    RwV3d lightDir = lightFrame->modelling.at;
    RwV3dNormalize(&lightDir, &lightDir);
    gCarEnvMapLight.Direction.x = lightDir.x;
    gCarEnvMapLight.Direction.y = lightDir.y;
    gCarEnvMapLight.Direction.z = lightDir.z;
    gCarEnvMapLight.Type = D3DLIGHT_DIRECTIONAL;
    gCarEnvMapLight.Diffuse.r = 0.25f;
    gCarEnvMapLight.Diffuse.g = 0.25f;
    gCarEnvMapLight.Diffuse.b = 0.25f;
    gCarEnvMapLight.Diffuse.a = 1.00f;
    gCarEnvMapLight.Ambient.r = 0.75f;
    gCarEnvMapLight.Ambient.g = 0.75f;
    gCarEnvMapLight.Ambient.b = 0.75f;
    gCarEnvMapLight.Ambient.a = 1.00f;
    gCarEnvMapLight.Specular.r = 0.65f;
    gCarEnvMapLight.Specular.g = 0.65f;
    gCarEnvMapLight.Specular.b = 0.65f;
    gCarEnvMapLight.Specular.a = 1.00f;
    gCarEnvMapLight.Range = 1000.0f;
    gCarEnvMapLight.Falloff = 0.0f;
    gCarEnvMapLight.Attenuation0 = 1.0f;
    gCarEnvMapLight.Attenuation1 = 0.0f;
    gCarEnvMapLight.Attenuation2 = 0.0f;
}

void* CCustomCarEnvMapPipeline::pluginEnvMatConstructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    PLUGIN_ENVMAP(object, data) = &m_fakeEnvMapPipeMatData;
    return object;
}

void* CCustomCarEnvMapPipeline::pluginEnvMatDestructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    CustomEnvMapPipeMaterialData* data = PLUGIN_ENVMAP(object, data);
    if(data != &m_fakeEnvMapPipeMatData)
    {
        m_gEnvMapPipeMatDataPool->Delete(data);
        PLUGIN_ENVMAP(object, data) = NULL;
    }
}

void *CCustomCarEnvMapPipeline::pluginEnvMatCopyConstructorCB(void* dst, const void* src, RwInt32 offset, RwInt32 size)
{
    CustomEnvMapPipeMaterialData* dstData = PLUGIN_ENVMAP(dst, data);
    CustomEnvMapPipeMaterialData* srcData = PLUGIN_ENVMAP(src, data);
    if(srcData)
    {
        if(!dstData)
        {
            dstData = m_gEnvMapPipeMatDataPool->New();
            if(!dstData)
            {
                PLUGIN_ENVMAP(dst, data) = &m_fakeEnvMapPipeMatData;
                return dst;
            }
            PLUGIN_ENVMAP(dst, data) = dstData;
        }
        memcpy(dstData, srcData, size);
    }
    return dst;
}

static void PackCustomEnvMapPipeMatData(const CustomEnvMapPipeMaterialDataFull* uncompressed, CustomEnvMapPipeMaterialData* compressed)
{
    compressed->scaleX = FloatToFixedPoint<8>(uncompressed->scaleX);
    compressed->scaleY = FloatToFixedPoint<8>(uncompressed->scaleY);
    compressed->transScaleX = FloatToFixedPoint<8>(uncompressed->transScaleX);
    compressed->transScaleY = FloatToFixedPoint<8>(uncompressed->transScaleY);
    compressed->shininess = FloatToFixedPoint<255>(uncompressed->shininess);
    compressed->texture = uncompressed->texture;
    compressed->renderFrameCounter = 0;
}

RwInt32 CCustomCarEnvMapPipeline::pluginEnvMatStreamGetSizeCB(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    return object ? sizeof(CustomEnvMapPipeMaterialDataFull) : 0;
}

RwStream* CCustomCarEnvMapPipeline::pluginEnvMatStreamReadCB(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    CustomEnvMapPipeMaterialDataFull buffer;
    RwStreamRead(stream, &buffer, binaryLength);
    CustomEnvMapPipeMaterialData data;
    PackCustomEnvMapPipeMatData(&buffer, &data);
    bool isFake = memcmp(&data, &m_fakeEnvMapPipeMatData, sizeof(m_fakeEnvMapPipeMatData)) == 0;
    CustomEnvMapPipeMaterialData* objData = PLUGIN_ENVMAP_STREAM(object, offsetInObject)->data;
    if(isFake)
    {
        objData = &m_fakeEnvMapPipeMatData;
    }
    else
    {
        objData = m_gEnvMapPipeMatDataPool->New();
        if(objData)
        {
            *objData = data;
        }
    }
    return stream;
}

static void UnpackCustomEnvMapPipeMatData(CustomEnvMapPipeMaterialDataFull* uncompressed, const CustomEnvMapPipeMaterialData* compressed)
{
    uncompressed->scaleX = FixedPointToFloat<8>(compressed->scaleX);
    uncompressed->scaleY = FixedPointToFloat<8>(compressed->scaleY);
    uncompressed->transScaleX = FixedPointToFloat<8>(compressed->transScaleX);
    uncompressed->transScaleY = FixedPointToFloat<8>(compressed->transScaleY);
    uncompressed->shininess = FixedPointToFloat<255>(compressed->shininess);
    uncompressed->texture = compressed->texture;
}

RwStream* CCustomCarEnvMapPipeline::pluginEnvMatStreamWriteCB(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    CustomEnvMapPipeMaterialData* data = PLUGIN_ENVMAP_STREAM(object, offsetInObject)->data;
    CustomEnvMapPipeMaterialDataFull buffer;
    UnpackCustomEnvMapPipeMatData(&buffer, data);
    RwStreamWrite(stream, &buffer, binaryLength);
    return stream;
}

CustomEnvMapPipeMaterialData* CCustomCarEnvMapPipeline::DuplicateCustomEnvMapPipeMaterialData(CustomEnvMapPipeMaterialData** data)
{
    if(*data == &m_fakeEnvMapPipeMatData)
    {
        *data = m_gEnvMapPipeMatDataPool->New();
        if(*data)
        {
            (*data)->scaleX = m_fakeEnvMapPipeMatData.scaleX;
            (*data)->scaleY = m_fakeEnvMapPipeMatData.scaleY;
            (*data)->transScaleX = m_fakeEnvMapPipeMatData.transScaleX;
            (*data)->transScaleY = m_fakeEnvMapPipeMatData.transScaleY;
            (*data)->shininess = m_fakeEnvMapPipeMatData.shininess;
            (*data)->pad3 = m_fakeEnvMapPipeMatData.pad3;
            (*data)->renderFrameCounter = m_fakeEnvMapPipeMatData.renderFrameCounter;
            (*data)->texture = m_fakeEnvMapPipeMatData.texture;
        }
    }
    return *data;
}

void* CCustomCarEnvMapPipeline::pluginEnvAtmConstructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    PLUGIN_ENVMAPATM(object, data) = NULL;
    return object;
}

void* CCustomCarEnvMapPipeline::pluginEnvAtmCopyConstructorCB(void* dst, const void* src, RwInt32 offset, RwInt32 size)
{
    CustomEnvMapPipeAtomicData* dstData = PLUGIN_ENVMAPATM(dst, data);
    CustomEnvMapPipeAtomicData* srcData = PLUGIN_ENVMAPATM(src, data);
    if(srcData)
    {
        if(!dstData)
        {
            dstData = m_gEnvMapPipeAtmDataPool->New();
            if(!dstData)
            {
                return dst;
            }
            PLUGIN_ENVMAPATM(dst, data) = dstData;
        }
        memcpy(dstData, srcData, sizeof(*srcData));
    }
    return dst;
}

void* CCustomCarEnvMapPipeline::pluginEnvAtmDestructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    CustomEnvMapPipeAtomicData* data = PLUGIN_ENVMAPATM(object, data);
    if(data)
    {
        m_gEnvMapPipeAtmDataPool->Delete(data);
        PLUGIN_ENVMAPATM(object, data) = NULL;
    }
    return object;
}

void* CCustomCarEnvMapPipeline::pluginSpecMatConstructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    PLUGIN_SPECMAP(object, data) = NULL;
    return object;
}

void* CCustomCarEnvMapPipeline::pluginSpecMatCopyConstructorCB(void* dst, const void* src, RwInt32 offset, RwInt32 size)
{
    CustomSpecMapPipeMaterialData* dstData = PLUGIN_SPECMAP(dst, data);
    CustomSpecMapPipeMaterialData* srcData = PLUGIN_SPECMAP(src, data);
    if(srcData)
    {
        if(!dstData)
        {
            dstData = m_gSpecMapPipeMatDataPool->New();
            if(!dstData)
            {
                return dst;
            }
            PLUGIN_SPECMAP(dst, data) = dstData;
        }
        *dstData = *srcData;
        if(dstData->texture)
        {
            dstData->texture->refCount++;
        }
    }
    return dst;
}

void* CCustomCarEnvMapPipeline::pluginSpecMatDestructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    CustomSpecMapPipeMaterialData* data = PLUGIN_SPECMAP(object, data);
    if(data)
    {
        if(data->texture)
        {
            RwTextureDestroy(data->texture);
            data->texture = NULL;
        }
        m_gSpecMapPipeMatDataPool->Delete(data);
        PLUGIN_SPECMAP(object, data) = NULL;

    }
    return object;
}

RwBool CCustomCarEnvMapPipeline::RegisterPlugin()
{
    ms_envMapPluginOffset = RpMaterialRegisterPlugin(sizeof(EnvMapPluginDef),
                                                     MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_ENVMAP),
                                                     pluginEnvMatConstructorCB,
                                                     pluginEnvMatDestructorCB,
                                                     pluginEnvMatCopyConstructorCB);
    if(ms_envMapPluginOffset == -1)
    {
        return false;
    }
    if(RpMaterialRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_ENVMAP),
                                      pluginEnvMatStreamReadCB,
                                      pluginEnvMatStreamWriteCB,
                                      pluginEnvMatStreamGetSizeCB) < 0)
    {
        ms_envMapPluginOffset = -1;
        return false;
    }
    m_fakeEnvMapPipeMatData.scaleX = FloatToFixedPoint<8>(1.0f);
    m_fakeEnvMapPipeMatData.scaleY = FloatToFixedPoint<8>(1.0f);
    m_fakeEnvMapPipeMatData.transScaleX = FloatToFixedPoint<8>(1.0f);
    m_fakeEnvMapPipeMatData.transScaleY = FloatToFixedPoint<8>(1.0f);
    m_fakeEnvMapPipeMatData.shininess = FloatToFixedPoint<255>(1.0f);
    m_fakeEnvMapPipeMatData.texture = NULL;
    m_fakeEnvMapPipeMatData.renderFrameCounter = 0;
    ms_envMapAtmPluginOffset = RpAtomicRegisterPlugin(sizeof(EnvMapAtmPluginDef),
                                                      MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_ENVMAPATM),
                                                      pluginEnvAtmConstructorCB,
                                                      pluginEnvAtmDestructorCB,
                                                      pluginEnvAtmCopyConstructorCB);
    if(ms_envMapAtmPluginOffset == -1)
    {
        return false;
    }
    ms_specularMapPluginOffset = RpMaterialRegisterPlugin(sizeof(SpecMapPluginDef),
                                                          MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_SPECMAP),
                                                          pluginSpecMatConstructorCB,
                                                          pluginSpecMatDestructorCB,
                                                          pluginSpecMatCopyConstructorCB);
    if(ms_specularMapPluginOffset == -1)
    {
        return false;
    }
    if(RpMaterialRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_SPECMAP),
                                      pluginSpecMatStreamReadCB,
                                      pluginSpecMatStreamWriteCB,
                                      pluginSpecMatStreamGetSizeCB) < 0)
    {
        ms_specularMapPluginOffset = -1;
        return false;
    }
    return true;
}

void CCustomCarEnvMapPipeline::SetFxEnvScale(RpMaterial* material, float scaleX, float scaleY)
{
    CustomEnvMapPipeMaterialData* data = DuplicateCustomEnvMapPipeMaterialData(&PLUGIN_ENVMAP(material, data));
    if(data)
    {
        data->scaleX = FloatToFixedPoint<8>(scaleX);
        data->scaleY = FloatToFixedPoint<8>(scaleY);
    }
}

void CCustomCarEnvMapPipeline::SetFxEnvTransScl(RpMaterial* material, float transScaleX, float transScaleY)
{
    CustomEnvMapPipeMaterialData* data = DuplicateCustomEnvMapPipeMaterialData(&PLUGIN_ENVMAP(material, data));
    if(data)
    {
        data->transScaleX = FloatToFixedPoint<8>(transScaleX);
        data->transScaleY = FloatToFixedPoint<8>(transScaleY);
    }
}

void CCustomCarEnvMapPipeline::SetFxEnvShininess(RpMaterial* material, float shininess)
{
    CustomEnvMapPipeMaterialData* data = DuplicateCustomEnvMapPipeMaterialData(&PLUGIN_ENVMAP(material, data));
    if(data)
    {
        data->shininess = FloatToFixedPoint<255>(shininess);
    }
}

void CCustomCarEnvMapPipeline::SetFxEnvTexture(RpMaterial* material, RwTexture* texture)
{
    CustomEnvMapPipeMaterialData* data = DuplicateCustomEnvMapPipeMaterialData(&PLUGIN_ENVMAP(material, data));
    if(data)
    {
        if(texture)
        {
            data->texture = texture;
        }
        else
        {
            data->texture = RpMatFXMaterialGetEnvMapTexture(material);
            if(!data->texture || FixedPointToFloat<255>(data->shininess) == 0.0f)
            {
                m_gEnvMapPipeMatDataPool->Delete(data);
                data = &m_fakeEnvMapPipeMatData;
                PLUGIN_ENVMAP(material, data) = data;
            }
            if(data->texture)
            {
                data->texture->filterAddressing = 0x1102;
            }
        }
    }
}

static bool D3D9SupportsCarEnvMapPipeline()
{
    const D3DCAPS9* caps = (const D3DCAPS9*)RwD3D9GetCaps();
    return caps->MaxTextureBlendStages >= 2 &&
           (caps->TextureOpCaps & D3DTEXOPCAPS_MULTIPLYADD) &&
           (caps->TextureOpCaps & D3DTEXOPCAPS_BLENDFACTORALPHA) &&
           caps->MaxSimultaneousTextures >= 2 &&
           (caps->ShadeCaps & D3DPSHADECAPS_SPECULARGOURAUDRGB);
}

bool CCustomCarEnvMapPipeline::CreatePipe()
{
    ObjPipeline = CreateCustomD3D9ObjPipe();
    if(!ObjPipeline || !D3D9SupportsCarEnvMapPipeline())
    {
        memset(&gCarEnvMapLight, 0, sizeof(gCarEnvMapLight));
        m_gEnvMapPipeMatDataPool = new CPool<CustomEnvMapPipeMaterialData>(4096);
        m_gEnvMapPipeAtmDataPool = new CPool<CustomEnvMapPipeAtomicData>(1024);
        m_gSpecMapPipeMatDataPool = new CPool<CustomSpecMapPipeMaterialData>(4096);
        return false;
    }
    return true;
}

void CCustomCarEnvMapPipeline::DestroyPipe()
{
    delete m_gEnvMapPipeAtmDataPool;
    m_gEnvMapPipeAtmDataPool = NULL;
    delete m_gEnvMapPipeMatDataPool;
    m_gEnvMapPipeMatDataPool = NULL;
    delete m_gSpecMapPipeMatDataPool;
    m_gSpecMapPipeMatDataPool = NULL;
}

struct CustomSpecMapPipeMaterialDataFile
{
    float specularity;
    char textureName[24];
};

RwInt32 CCustomCarEnvMapPipeline::pluginSpecMatStreamGetSizeCB(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    return ms_specularMapPluginOffset == -1 ? 0 : sizeof(CustomSpecMapPipeMaterialDataFile);
}

RwStream* CCustomCarEnvMapPipeline::pluginSpecMatStreamReadCB(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    CustomSpecMapPipeMaterialDataFile buffer;
    RwStreamRead(stream, &buffer, binaryLength);
    if(buffer.specularity != 0.0f)
    {
        RwTexture* texture = RwTextureRead(buffer.textureName, NULL);
        if(texture)
        {
            CustomSpecMapPipeMaterialData* data = m_gSpecMapPipeMatDataPool->New();
            PLUGIN_SPECMAP_STREAM(object, offsetInObject)->data = data;
            if(data)
            {
                data->specularity = buffer.specularity;
                data->texture = texture;
            }
        }
    }
    return stream;
}

RwStream* CCustomCarEnvMapPipeline::pluginSpecMatStreamWriteCB(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    CustomSpecMapPipeMaterialDataFile buffer;
    memset(&buffer, 0, sizeof(buffer));
    CustomSpecMapPipeMaterialData* data = PLUGIN_SPECMAP_STREAM(stream, offsetInObject)->data;
    if(data)
    {
        buffer.specularity = data->specularity;
        if(data->texture)
        {
            strncpy(buffer.textureName, data->texture->name, sizeof(buffer.textureName));
        }
    }
    RwStreamWrite(stream, &buffer, binaryLength);
    return stream;
}

void CCustomCarEnvMapPipeline::SetFxSpecSpecularity(RpMaterial* material, float specularity)
{
    CustomSpecMapPipeMaterialData* data = PLUGIN_SPECMAP(material, data);
    if(data)
    {
        data->specularity = specularity;
    }
}

void CCustomCarEnvMapPipeline::SetFxSpecTexture(RpMaterial* material, RwTexture* texture)
{
    CustomSpecMapPipeMaterialData* data = PLUGIN_SPECMAP(material, data);
    if(data)
    {
        data->texture = texture;
    }
}

float CCustomCarEnvMapPipeline::GetFxSpecSpecularity(RpMaterial* material)
{
    CustomSpecMapPipeMaterialData* data = PLUGIN_SPECMAP(material, data);
    if(!data)
    {
        return 0.0f;
    }
    return data->specularity;
}

RwTexture* CCustomCarEnvMapPipeline::GetFxSpecTexture(RpMaterial* material)
{
    CustomSpecMapPipeMaterialData* data = PLUGIN_SPECMAP(material, data);
    if(data)
    {
        return data->texture;
    }
    return NULL;
}

RpMaterial* CCustomCarEnvMapPipeline::CustomPipeMaterialSetup(RpMaterial* material, void* data)
{
    RwUInt32* materialFlags = (RwUInt32*)&(material->surfaceProps.specular);
    material->surfaceProps.specular = 0;
    if(RpMatFXMaterialGetEffects(material) == rpMATFXEFFECTENVMAP)
    {
        SetFxEnvTexture(material, NULL);
    }
    CustomEnvMapPipeMaterialData* envMapData = PLUGIN_ENVMAP(material, data);
    if(envMapData && FixedPointToFloat<255>(envMapData->shininess) != 0.0f && envMapData->texture)
    {
        if(envMapData->texture->name[0] == 'x')
        {
            *materialFlags |= 2;
        }
        else
        {
            *materialFlags |= 1;
        }
    }
    CustomSpecMapPipeMaterialData* specMapData = PLUGIN_SPECMAP(material, data);
    if(specMapData && specMapData->specularity != 0.0f && specMapData->texture)
    {
        *materialFlags |= 4;
    }
    return material;
}

void CCustomCarEnvMapPipeline::CustomPipeAtomicSetup(RpAtomic* atomic)
{
    RpGeometryForAllMaterials(atomic->geometry, CustomPipeMaterialSetup, &atomic);
    atomic->pipeline = ObjPipeline;
    SetPipelineID(atomic, CUSTOM_CAR_ENV_MAP_PIPELINE);
}