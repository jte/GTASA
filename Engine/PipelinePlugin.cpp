#include "StdInc.h"

#define rwID_PIPELINE (0xF3)

#define PLUGIN_PIPELINE(object) \
    (*RWPLUGINOFFSET(RwUInt32, object, gPipelinePlugin))

static RwInt32 gPipelinePlugin = -1;

static void* PipelinePluginConstructor(void *object, RwInt32 offset, RwInt32 size)
{
    PLUGIN_PIPELINE(object) = 0;
    return object;
}

static void* PipelinePluginCopyConstructor(void *dst, const void *src, RwInt32 offset, RwInt32 size)
{
    PLUGIN_PIPELINE(dst) = PLUGIN_PIPELINE(src);
    return dst;
}


RwInt32 PipelinePluginGetSize(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    return sizeof(RwUInt32);
}

RwStream* PipelinePluginRead(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    RwUInt32 buffer;
    RwStreamRead(stream, &buffer, binaryLength);
    PLUGIN_PIPELINE(object) = buffer;
    return stream;
}


RwBool PipelinePluginAttach()
{
    gPipelinePlugin = RpAtomicRegisterPlugin(sizeof(RwUInt32), 
                                             MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_PIPELINE), 
                                             PipelinePluginConstructor, 
                                             NULL, 
                                             PipelinePluginCopyConstructor);
    if(gPipelinePlugin == -1)
    {
        return false;
    }
    if(RpAtomicRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_PIPELINE), PipelinePluginRead, 0, PipelinePluginGetSize) < 0 )
    {
        gPipelinePlugin = -1;
        return false;
    }
    return true;
}

RwUInt32 GetPipelineID(RpAtomic* atomic)
{
    return PLUGIN_PIPELINE(atomic);
}

void SetPipelineID(RpAtomic* atomic, RwUInt32 id)
{
    PLUGIN_PIPELINE(atomic) = id;
}