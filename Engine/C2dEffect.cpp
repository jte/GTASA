#include "StdInc.h"

#define rwID_2DEFFECT  (0xF8)

#define PLUGIN_2DEFFECT(object, var) \
    (RWPLUGINOFFSET(Plugin2dEffectDef, object, g2dEffectPluginOffset)->var)

static RwInt32 g2dEffectPluginOffset = -1;

struct Plugin2dEffectDef
{
    void* data;
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
}

static RwStream* Effect2dPluginRead(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
}

static RwStream* Effect2dPluginWrite(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
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