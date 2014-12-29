#pragma once

class CCarFXRenderer
{
public:
	static RpAtomic* AtomicRemapDirtCB(RpAtomic* atomic, void* data);
	static void CustomCarPipeAtomicSetup(RpAtomic* atomic);
	static void CustomCarPipeClumpSetup(RpClump* clump);
	static float GetFxEnvMapLightMult();
	static bool Initialise();
	static void InitialiseDirtTexture();
	static bool IsCCPCPipelineAttached(RpAtomic* atomic);
	static RpMaterial* MaterialRemapDirtCB(RpMaterial* material, void* data);
	static void  PreRenderUpdate();
	static bool RegisterPlugins();
	static void RemapDirt(CVehicleModelInfo* info, uint32_t id);
	static void SetCustomFXAtomicRenderPipelinesVMICB(RpAtomic* atomic, void*);
	static void SetFxEnvMapLightMult(float mult);
	static void Shutdown();
protected:
private:
    static RwTexture* ms_aDirtTextures[16];
};