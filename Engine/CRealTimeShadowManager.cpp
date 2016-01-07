#include "StdInc.h"

CRealTimeShadow* CRealTimeShadowManager::ReturnRealTimeShadow(CRealTimeShadow* shadow)
{
	CRealTimeShadow *result; // eax@2

	if (m_initialized)
	{
		result = shadow;
		shadow->m_shadowedObject->pShadowData = 0;
		shadow->m_shadowedObject = 0;
	}
	return result;
}

void CRealTimeShadowManager::Init()
{
	if (m_initialized)
	{
		return;
	}
	for (size_t i = 0; i < ELEMS_COUNT(m_realTimeShadowPool); i++)
	{
		m_realTimeShadowPool[i] = new CRealTimeShadow;
		m_realTimeShadowPool[i]->Create(true, 4, true);
	}
	m_camera1.Create(6);
	m_camera2.Create(6);
	m_camera2.MakeGradientRaster();
	m_initialized = true;
}