#include "StdInc.h"

void CVehicle::ResetAfterRender()
{
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	CVehicleModelInfo::ResetEditableMaterials((RpClump*)GetRwObject());
	if (uiVehicleType == 0)
	{
		CVehicleModelInfo* modelInfo = (CVehicleModelInfo*)CModelInfo::GetModelInfo(GetModelIndex());
		CAutomobile::CustomCarPlate_AfterRenderingStop(modelInfo);
	}
}

void CVehicle::Render()
{
	CVehicleModelInfo* modelInfo = (CVehicleModelInfo*)CModelInfo::GetModelInfo(GetModelIndex());
	CCarFXRenderer::RemapDirt(modelInfo, this->uiBodyDirtLevel & 15);
	CEntity::Render();
}

void CVehicle::PreRender()
{
	if (this->uiVehicleType != 6)
	{
		CalculateLightingFromCollision(this);
	}
	PreRenderDriverAndPassengers(pThis);
	CVehicleModelInfo* modelInfo = (CVehicleModelInfo*)CModelInfo::GetModelInfo(GetModelIndex());
	if (modelInfo->Get2dEffectCount() != 0)
	{
		CEntity::ProcessLightsForEntity();
	}
	LOBYTE(pThis->uiRenderLightsFlags_) &= 0xF0u;
	float envMapCoeff = CPhysical::GetLightingFromCol(0) * 0.4;
	return modelInfo->SetEnvMapCoeff(envMapCoeff);
}

void CVehicle::DeleteRwObject()
{
	SetRemapTexDictionary(0);
	RemoveAllUpgrades();
	CEntity::DeleteRwObject();
}

bool CVehicle::CustomCarPlate_TextureCreate(CVehicleModelInfo* modelInfo)
{
  this->pLicenseplateTexture = 0;
  if ( modelInfo->pLicenseplateMaterial )
  {
    v3 = CVehicleModelInfo::GetCustomCarPlateText(modelInfo);
    if ( v3 )
    {
      pThis->pLicenseplateTexture = CCustomCarPlateMgr::CreatePlateTexture((char *)v3, modelInfo->ucPlateDesign);
      CVehicleModelInfo::SetCustomCarPlateText(modelInfo, 0);
      modelInfo->ucPlateDesign = -1;
      result = 1;
    }
    else
    {
      v5 = modelInfo->pLicenseplateMaterial->texture;
      ++v5->refCount;
      pThis->pLicenseplateTexture = v5;
      result = 1;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}