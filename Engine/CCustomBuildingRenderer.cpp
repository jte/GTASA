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

bool CCustomBuildingRenderer::IsCBPCPipelineAttached(RpAtomic* atomic)
{
  int v1; // eax@1
  RpGeometry *v2; // esi@1

	v1 = GetPipelineID(atomic);
	v2 = atomic->geometry;
	return v1 == 0x53F2009C || v1 == 0x53F20098 || CCustomBuildingDNPipeline::GetExtraVertColourPtr(v2) && v2->preLitLum;
}

void CCustomBuildingRenderer::UpdateDayNightBalanceParam()
{
  double v0; // st7@1
  float v1; // [sp+0h] [bp-4h]@1

  v0 = (double)((unsigned __int8)CClock::ms_nGameClockMinutes + 60 * (unsigned __int8)CClock__ms_nGameClockHours)
     + (double)(unsigned __int16)CClock::ms_nGameClockSeconds * 0.016666668;
  v1 = v0;
  if ( v0 < 360.0 )
    goto LABEL_11;
  if ( v1 < 420.0 )
  {
    CCustomBuildingDNPipeline::m_fDNBalanceParam = (420.0 - v1) * 0.016666668;
    return;
  }
  if ( v1 < 1200.0 )
  {
    CCustomBuildingDNPipeline::m_fDNBalanceParam = 0.0;
    return;
  }
  if ( v1 >= 1260.0 )
LABEL_11:
    CCustomBuildingDNPipeline::m_fDNBalanceParam = 1.0;
  else
    CCustomBuildingDNPipeline::m_fDNBalanceParam = 1.0 - (1260.0 - v1) * 0.016666668;
}