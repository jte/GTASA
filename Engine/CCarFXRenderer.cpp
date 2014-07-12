#include "StdInc.h"

bool CCarFXRenderer::Initialise()
{
    return CCustomCarEnvMapPipeline::CreatePipe();
}

void CCarFXRenderer::Shutdown()
{
    for(size_t i = 0; i < sizeof(*ms_aDirtTextures); i++)
    {
        RwTextureDestroy(ms_aDirtTextures[i]);
    }
    CCustomCarEnvMapPipeline::DestroyPipe();
}

bool CCarFXRenderer::RegisterPlugins()
{
    return CCustomCarEnvMapPipeline::RegisterPlugin();
}

void CCarFXRenderer::PreRenderUpdate()
{
    return CCustomCarEnvMapPipeline::PreRenderUpdate();
}

bool CCarFXRenderer::IsCCPCPipelineAttached(RpAtomic* atomic)
{
    return GetPipelineID(atomic) == 0x53F2009A;
}

static RpAtomic* CustomCarPipeAtomicSetupCB(RpAtomic* atomic, void*)
{
    CCarFXRenderer::CustomCarPipeAtomicSetup(atomic);
    return atomic;
}

void CCarFXRenderer::CustomCarPipeAtomicSetup(RpAtomic* atomic)
{
    CCustomCarEnvMapPipeline::CustomPipeAtomicSetup(atomic);
}

void CCarFXRenderer::CustomCarPipeClumpSetup(RpClump* clump)
{
    RpClumpForAllAtomics(clump, CustomCarPipeAtomicSetupCB, NULL);
}

void CCarFXRenderer::SetFxEnvMapLightMult(float mult)
{
    CCustomCarEnvMapPipeline::m_EnvMapLightingMult = mult;
}

float CCarFXRenderer::GetFxEnvMapLightMult()
{
    return CCustomCarEnvMapPipeline::m_EnvMapLightingMult;
}

void CCarFXRenderer::RemapDirt(CVehicleModelInfo* info, uint32_t id)
{
    for(size_t i = 0; i < ELEMS_COUNT(info->DirtMaterialArray); i++)
    {
        RpMaterial* dirtMaterial = info->DirtMaterialArray[i];
        if(dirtMaterial)
        {
            RpMaterialSetTexture(dirtMaterial, ms_aDirtTextures[id]);
        }
    }
}

void CCarFXRenderer::SetCustomFXAtomicRenderPipelinesVMICB(RpAtomic* atomic, void*)
{
    CCarFXRenderer::CustomCarPipeAtomicSetup(atomic);
}

RpAtomic* CCarFXRenderer::AtomicRemapDirtCB(RpAtomic* atomic, void* data)
{
    RpGeometryForAllMaterials(atomic->geometry, MaterialRemapDirtCB, data);
}

RpMaterial* CCarFXRenderer::MaterialRemapDirtCB(RpMaterial* material, void* data)
{
    if(material->texture)
    {
        if(!strcmp(material->texture->name, "vehiclegrunge256"))
        {
            RpMaterialSetTexture(material, ms_aDirtTextures[*(int*)data]);
        }
    }
    return material;
}
