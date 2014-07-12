#include "StdInc.h"

bool CCustomBuildingRenderer::Initialise()
{
    return CCustomBuildingPipeline::CreatePipe() && CCustomBuildingDNPipeline::CreatePipe();
}

void CCustomBuildingRenderer::Shutdown()
{
    CCustomBuildingPipeline::DestroyPipe();
    CCustomBuildingDNPipeline::DestroyPipe();
}

bool CCustomBuildingRenderer::PluginAttach()
{
    return CCustomBuildingDNPipeline::ExtraVertColourPluginAttach();
}

void CCustomBuildingRenderer::AtomicSetup(RpAtomic* atomic)
{
    RpGeometry* geometry = atomic->geometry;

    if(CCustomBuildingDNPipeline::GetExtraVertColourPtr(geometry) && geometry->preLitLum)
    {
        CCustomBuildingDNPipeline::CustomPipeAtomicSetup(atomic);
    }
    else
    {
        CCustomBuildingPipeline::CustomPipeAtomicSetup(atomic);
    }
}