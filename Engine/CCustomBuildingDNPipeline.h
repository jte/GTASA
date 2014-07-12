#pragma once

class CCustomBuildingDNPipeline
{
public:
	//* CCustomBuildingDNPipeline()
	//* CreateCustomObjPipe()
	//* CreatePipe()
	//* CustomPipeAtomicSetup(RpAtomic*)
	//* CustomPipeInstanceCB(void*, RxOpenGLMeshInstanceData*, int, int)
	//* CustomPipeMaterialSetup(RpMaterial*, void*)
	//* CustomPipeRenderCB(RwResEntry*, void*, unsigned char, unsigned int)
	//* DestroyPipe()
	static bool ExtraVertColourPluginAttach();
	//* GetExtraVertColourPtr(RpGeometry*)
	//* GetFxEnvScaleX(RpMaterial*)
	//* GetFxEnvScaleY(RpMaterial*)
	//* GetFxEnvShininess(RpMaterial*)
	//* GetFxEnvTexture(RpMaterial*)
	//* GetFxEnvTransSclX(RpMaterial*)
	//* GetFxEnvTransSclY(RpMaterial*)
	//* SetFxEnvScale(RpMaterial*, float, float)
	//* SetFxEnvShininess(RpMaterial*, float)
	//* SetFxEnvTexture(RpMaterial*, RwTexture*)
	//* SetFxEnvTransScl(RpMaterial*, float, float)
	//* UsesThisPipeline(RpAtomic*)
    static void* pluginExtraVertColourConstructorCB(void* object, RwInt32 offset, RwInt32 size);
	static void* pluginExtraVertColourDestructorCB(void* object, RwInt32 offset, RwInt32 size);
	static RwInt32 pluginExtraVertColourStreamGetSizeCB(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream* pluginExtraVertColourStreamReadCB(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream* pluginExtraVertColourStreamWriteCB(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	//* ~CCustomBuildingDNPipeline()

    //
    static void SetPrelitColors(RpAtomic* atomic, float dnParam);
protected:
private:
	static RxPipeline* ObjPipeline;
	//* m_AtmDNWorkingIndex
	//* m_bCameraChange
	//* m_bDeviceSupportsVS11
    /**
        m_fDNBalanceParam -> night lighting
        (1.0f - m_fDNBalanceParam) -> day lighting
     */
	static float m_fDNBalanceParam;
	static RwInt32 ms_extraVertColourPluginOffset;
};