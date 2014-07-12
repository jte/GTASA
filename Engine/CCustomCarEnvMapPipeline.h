/**
 *  Note: "RpMaterial->specular" is not used by RenderWare and R*
 *          uses it for flags
 */
#pragma once

#define CUSTOM_CAR_ENV_MAP_PIPELINE (0x53F2009A)  

/**
 * Internal (compressed) representation of CustomEnvMapPipeMaterialData
 */
struct CustomEnvMapPipeMaterialData
{
    FixedPoint8 scaleX;
    FixedPoint8 scaleY;
    FixedPoint8 transScaleX;
    FixedPoint8 transScaleY;
    FixedPoint8 shininess;
    uint8_t pad3;
    uint16_t renderFrameCounter;
    RwTexture* texture;
};

/**
 * File (uncompressed) representation of CustomEnvMapPipeMaterialData
 */
struct CustomEnvMapPipeMaterialDataFull
{
    float scaleX;
    float scaleY;
    float transScaleX;
    float transScaleY;
    float shininess;
    RwTexture* texture;
};

struct CustomEnvMapPipeAtomicData
{
    int pad1;
    int pad2;
    int pad3;
};

struct CustomSpecMapPipeMaterialData
{
    float specularity;
    RwTexture* texture;
};

class CCustomCarEnvMapPipeline
{
public:
	static CustomEnvMapPipeAtomicData* AllocEnvMapPipeAtomicData(RpAtomic* atomic); // TODO
	//* CCustomCarEnvMapPipeline()
	static RxPipeline* CreateCustomD3D9ObjPipe();
	static bool CreatePipe();
	static void CustomPipeAtomicSetup(RpAtomic* atomic);
	static RwBool CustomPipeReInstanceCB(void* object, RwResEntry* resEntry, RxD3D9AllInOneInstanceCallBack instanceCallback);
    static RpMaterial* CustomPipeMaterialSetup(RpMaterial* material, void* data);
	static void CustomPipeRenderCB(RwResEntry* repEntry, void* object, RwUInt8 type, RwUInt32 flags);
	static void DestroyPipe();
	static CustomEnvMapPipeMaterialData* DuplicateCustomEnvMapPipeMaterialData(CustomEnvMapPipeMaterialData** data);
	//* GetFxEnvScaleX(RpMaterial*)
	//* GetFxEnvScaleY(RpMaterial*)
	//* GetFxEnvShininess(RpMaterial*)
	//* GetFxEnvTexture(RpMaterial*)
	//* GetFxEnvTransSclX(RpMaterial*)
	//* GetFxEnvTransSclY(RpMaterial*)
	static float GetFxSpecSpecularity(RpMaterial* material);
	static RwTexture* GetFxSpecTexture(RpMaterial* material);
	static void PreRenderUpdate();
	static RwBool RegisterPlugin();
	//* SetCustomEnvMapPipeAtomicDataDefaults(CustomEnvMapPipeAtomicData*)
	//* SetCustomEnvMapPipeMaterialDataDefaults(CustomEnvMapPipeMaterialData*)
	static void SetFxEnvScale(RpMaterial* material, float scaleX, float scaleY);
	static void SetFxEnvShininess(RpMaterial* material, float shininess);
	static void SetFxEnvTexture(RpMaterial* material, RwTexture* texture);
	static void SetFxEnvTransScl(RpMaterial* material, float transScaleX, float transScaleY);
	static void SetFxSpecSpecularity(RpMaterial* material, float specularity);
	static void SetFxSpecTexture(RpMaterial* material, RwTexture* texture);
	static void* pluginEnvAtmConstructorCB(void* object, RwInt32 offset, RwInt32 size);
	static void* pluginEnvAtmCopyConstructorCB(void* dst, const void* src, RwInt32 offset, RwInt32 size);
	static void* pluginEnvAtmDestructorCB(void* object, RwInt32 offset, RwInt32 size);
	static void* pluginEnvMatConstructorCB(void* object, RwInt32 offset, RwInt32 size);
	static void* pluginEnvMatDestructorCB(void* object, RwInt32 offset, RwInt32 size);
	static void* pluginEnvMatCopyConstructorCB(void* dst, const void* src, RwInt32 offset, RwInt32 size);
	static RwInt32 pluginEnvMatStreamGetSizeCB(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream* pluginEnvMatStreamReadCB(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream* pluginEnvMatStreamWriteCB(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static void* pluginSpecMatConstructorCB(void* object, RwInt32 offset, RwInt32 size);
	static void* pluginSpecMatCopyConstructorCB(void* dst, const void* src, RwInt32 offset, RwInt32 size);
	static void* pluginSpecMatDestructorCB(void* object, RwInt32 offset, RwInt32 size);
	static RwInt32 pluginSpecMatStreamGetSizeCB(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream* pluginSpecMatStreamReadCB(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream* pluginSpecMatStreamWriteCB(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	//* ~CCustomCarEnvMapPipeline()
protected:
private:
    static RxPipeline* ObjPipeline;
	static CustomEnvMapPipeMaterialData m_fakeEnvMapPipeMatData; // placeholder
	static float m_EnvMapLightingMult;
	static CPool<CustomEnvMapPipeAtomicData>* m_gEnvMapPipeAtmDataPool;
	static CPool<CustomEnvMapPipeMaterialData>* m_gEnvMapPipeMatDataPool;
	static CPool<CustomSpecMapPipeMaterialData>* m_gSpecMapPipeMatDataPool;
	static RwInt32 ms_envMapAtmPluginOffset;
	static RwInt32 ms_envMapPluginOffset;
	static RwInt32 ms_specularMapPluginOffset;
};