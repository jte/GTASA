#include "StdInc.h"

#define rwID_BREAKABLE (0xFD)

#define PLUGIN_BREAKABLE(geometry, var) \
    (RWPLUGINOFFSET(BreakablePluginDef, geometry, g_BreakablePlugin)->var)

struct BreakablePluginDef
{
    BreakInfo_t* data;
};

static RwInt32 g_BreakablePlugin = -1;

void* BreakableConstructor(void* object, int, int)
{
    if (g_BreakablePlugin > 0)
    {
        PLUGIN_BREAKABLE(object, data) = NULL;
    }
    return object;
}

bool BreakablePluginAttach()
{
    g_BreakablePlugin = RpGeometryRegisterPlugin(4, 
                                                 MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_BREAKABLE), 
                                                 BreakableConstructor, 
                                                 BreakableDestructor, 
                                                 NULL);
    if (g_BreakablePlugin != -1)
    {
        if (RpGeometryRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_BREAKABLE),
                                            BreakableStreamRead,
                                            BreakableStreamWrite,
                                            BreakableStreamGetSize) < 0)
        {
            g_BreakablePlugin = -1;
            return 0;
        }
    }
    return g_BreakablePlugin != -1;
}