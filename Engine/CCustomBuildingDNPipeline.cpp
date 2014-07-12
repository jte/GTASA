#include "StdInc.h"

#define rwID_EXTRAVERTCOLOUR (0xF9)

#define PLUGIN_EXTRAVERTCOLOUR(object, var) \
    (RWPLUGINOFFSET(ExtraVertColourPlugin, object, ms_extraVertColourPluginOffset)->var)

struct ExtraVertColourPlugin
{
    RwRGBA* nightColors; // array of RwRGBAs per vertice
    RwRGBA* dayColors; // array of RwRGBAs per vertice
    float dnParam;
};

void CCustomBuildingDNPipeline::SetPrelitColors(RpAtomic* atomic, float dnParam)
{
    RpGeometry* geometry = atomic->geometry;
    PLUGIN_EXTRAVERTCOLOUR(geometry, dnParam) = dnParam;
    RwRGBA* dayColors = PLUGIN_EXTRAVERTCOLOUR(geometry, dayColors);
    RwRGBA* nightColors = PLUGIN_EXTRAVERTCOLOUR(geometry, nightColors);
    RpGeometry* lockedGeometry = RpGeometryLock(geometry, 8);
    Clamp(0.0f, dnParam, 1.0f);
    for(size_t i = 0; i < geometry->numVertices; i++)
    {
        geometry->preLitLum[i].red   = dayColors[i].red   * (1.0 - dnParam) + nightColors[i].red   * dnParam;
        geometry->preLitLum[i].green = dayColors[i].green * (1.0 - dnParam) + nightColors[i].green * dnParam;
        geometry->preLitLum[i].blue  = dayColors[i].blue  * (1.0 - dnParam) + nightColors[i].blue  * dnParam;
        geometry->preLitLum[i].alpha = dayColors[i].alpha * (1.0 - dnParam) + nightColors[i].alpha * dnParam;
    }
    RpGeometryUnlock(lockedGeometry);
}

bool CCustomBuildingDNPipeline::ExtraVertColourPluginAttach()
{
    ms_extraVertColourPluginOffset = RpGeometryRegisterPlugin(sizeof(ExtraVertColourPlugin),
                                                              MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_EXTRAVERTCOLOUR),
                                                              pluginExtraVertColourConstructorCB,
                                                              pluginExtraVertColourDestructorCB,
                                                              0);
    if(ms_extraVertColourPluginOffset == -1)
    {
        return false;
    }
    if(RpGeometryRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_EXTRAVERTCOLOUR),
                                      pluginExtraVertColourStreamReadCB,
                                      pluginExtraVertColourStreamWriteCB,
                                      pluginExtraVertColourStreamGetSizeCB) < 0 )
    {
        ms_extraVertColourPluginOffset = -1;
        return false;
    }
    return ms_extraVertColourPluginOffset != -1;
}

void* CCustomBuildingDNPipeline::pluginExtraVertColourConstructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    if(ms_extraVertColourPluginOffset != -1)
    {
        PLUGIN_EXTRAVERTCOLOUR(object, nightColors) = NULL;
        PLUGIN_EXTRAVERTCOLOUR(object, dayColors) = NULL;
        PLUGIN_EXTRAVERTCOLOUR(object, dnParam) = 0.0f;
    }
    return object;
}

void* CCustomBuildingDNPipeline::pluginExtraVertColourDestructorCB(void* object, RwInt32 offset, RwInt32 size)
{
    RwRGBA* nightColors = PLUGIN_EXTRAVERTCOLOUR(object, nightColors);
    if(nightColors)
    {
        CMemoryMgr::Free(nightColors);
    }
    RwRGBA* dayColors = PLUGIN_EXTRAVERTCOLOUR(object, dayColors);
    if(dayColors)
    {
        CMemoryMgr::Free(dayColors);
    }
    return object;
}

RwInt32 CCustomBuildingDNPipeline::pluginExtraVertColourStreamGetSizeCB(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    RpGeometry* geometry = (RpGeometry*)object;
    return geometry ? geometry->numVertices * sizeof(RwRGBA) * 2 + 4 : 0;
}

RwStream* CCustomBuildingDNPipeline::pluginExtraVertColourStreamReadCB(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    float buffer;
    RwStreamRead(stream, &buffer, sizeof(buffer));
    if(buffer)
    {
        RpGeometry* geometry = (RpGeometry*)object;
        RwRGBA** nightColors = &PLUGIN_EXTRAVERTCOLOUR(object, nightColors);
        RwRGBA** dayColors = &PLUGIN_EXTRAVERTCOLOUR(object, dayColors);
        float* dnParam = &PLUGIN_EXTRAVERTCOLOUR(object, dnParam);
        *nightColors = (RwRGBA*)CMemoryMgr::Malloc(geometry->numVertices * sizeof(RwRGBA));
        *dayColors = (RwRGBA*)CMemoryMgr::Malloc(geometry->numVertices * sizeof(RwRGBA));
        *dnParam = 1.0f;
        RwStreamRead(stream, *dayColors, sizeof(RwRGBA) * geometry->numVertices);
        if(geometry->preLitLum)
        {
            for(size_t i = 0; i < geometry->numVertices; i++)
            {
                (*nightColors)[i].red = geometry->preLitLum[i].red;
                (*nightColors)[i].green = geometry->preLitLum[i].green;
                (*nightColors)[i].blue = geometry->preLitLum[i].blue;
                (*nightColors)[i].alpha = geometry->preLitLum[i].alpha;
            }
        }
    }
    return stream;
}

RwStream* CCustomBuildingDNPipeline::pluginExtraVertColourStreamWriteCB(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    RwStreamWrite(stream, PLUGIN_EXTRAVERTCOLOUR(object, nightColors), sizeof(RwRGBA*));
    if(PLUGIN_EXTRAVERTCOLOUR(object, nightColors))
    {
        RpGeometry* geometry = (RpGeometry*)object;
        RwStreamWrite(stream, (void*)*(int*)PLUGIN_EXTRAVERTCOLOUR(object, nightColors), geometry->numVertices * sizeof(RwRGBA));
    }
}