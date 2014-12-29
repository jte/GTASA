#include "StdInc.h"

bool CCarFXRenderer::Initialise()
{
    return CCustomCarEnvMapPipeline::CreatePipe();
}

void CCarFXRenderer::Shutdown()
{
    for(size_t i = 0; i < ELEMS_COUNT(ms_aDirtTextures); i++)
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

void CCarFXRenderer::InitialiseDirtTexture()
{
    int16_t id = CTxdStore::FindTxdSlot("vehicle");
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(id);
    RwTexture* texture = RwTextureRead("vehiclegrunge256", 0);
    RwRaster* raster = texture->raster;
    texture->filterAddressing = 2;
    for (size_t texId = 0; texId < ELEMS_COUNT(ms_aDirtTextures); texId++)
    {
        ms_aDirtTextures[texId] = CClothesBuilder::CopyTexture(texture);
        RwTextureSetName(ms_aDirtTextures[texId], "vehiclegrunge256");
        RwRaster* dirtRaster = ms_aDirtTextures[texId]->raster;
        RwUInt8** dirtArray = (RwUInt8**)RwRasterLock(dirtRaster, 0, 3);
        for (size_t i = 0; i != raster->height; i++)
        {
            for (size_t j = 0; j != raster->width; j++)
            {
                dirtArray[i][j]   = ((texId * 255) / 16) + ((texId * dirtArray[i][j])   / 16);
                dirtArray[i][j+1] = ((texId * 255) / 16) + ((texId * dirtArray[i][j+1]) / 16);
                dirtArray[i][j+2] = ((texId * 255) / 16) + ((texId * dirtArray[i][j+2]) / 16);
            }
        }
        RwRasterUnlock(dirtRaster);
    }
    CTxdStore::PopCurrentTxd();
}