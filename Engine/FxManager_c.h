#pragma once

#include "List_c.h"
#include "FxManager_c.h"
#include "FxFrustumInfo_c.h"

class FxSystem_c;
class FxSystemBP_c;
class FxFrustumInfo_c;

class FxManager_c
{
public:
    void CalcFrustumInfo(RwCamera* camera);
    FxSystem_c* CreateFxSystem(FxSystemBP_c*, RwMatrixTag*, RwMatrixTag*, unsigned char);
	/* CreateFxSystem(FxSystemBP_c*, RwV3d*, RwMatrixTag*, unsigned char)
	* CreateFxSystem(char*, RwMatrixTag*, RwMatrixTag*, unsigned char)
	* CreateFxSystem(char*, RwV3d*, RwMatrixTag*, unsigned char)
	* DestroyAllFxSystems()
	* DestroyFxSystem(FxSystem_c*)
	* Exit()
	* FreeUpParticle()
	* FxManager_c()
	*/ 
    RwMatrix* FxRwMatrixCreate();
	void FxRwMatrixDestroy(RwMatrixTag*);
	/* GetParticle(signed char)
	* Init()
	*/ 
    void LoadFxSystemBP(char* fileName, int file);
    void Render(RwCamera*, unsigned char);
	/* ReturnParticle(FxEmitterPrt_c*)
	*/ 
    void SetWindData(RwV3d* windVector, float* windStrength);
	bool ShouldCreate(FxSystemBP_c* fxSystemBP, RwMatrix* offsetMatrix, RwMatrix* parentMatrix, bool forceCreate);
	/* UnloadFxProject()
	* Update(RwCamera*, float)
	* ~FxManager_c()
    */
    bool LoadFxProject(char* fileName);
    FxSystemBP_c* FindFxSystemBP(char* name);
    FxFrustumInfo_c* GetFrustumInfo();
    FxMemoryPool_c GetMemoryPool();
private:
    List_c m_fxSystemBPList; // list o FxSystemBP_c
    List_c m_fxSystemList; // list of FxSystem_c 
    uint32_t pad1;
    List_c m_particles; // list of FxEmitterPrt_c
    uint32_t m_txdIndex;
    RwV3d* m_windVector;
    float m_windStrength;
    FxFrustumInfo_c m_frustumInfo;
    uint32_t m_currentMatrix;
    RwMatrixTag* m_matrices[8];
    FxMemoryPool_c m_buffer;
};