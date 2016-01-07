#include "StdInc.h"

int CWaterLevel::AddWaterLevelVertex(int x, int y, CRenPar renPar)
{
	float fx;
	float fy;

	if (x > MIN_WORLD_BOUNDS)
	{
		fx = x;
	}
	else
	{
		fx = MIN_WORLD_BOUNDS;
		x = MIN_WORLD_BOUNDS;
		renPar.z = 0.0f;
		renPar.u = 1.0f;
		renPar.waveHeight = 0.0f;
		renPar.velocity = 0.0f;
	}
	if (x >= MAX_WORLD_BOUNDS)
	{
		fx = MAX_WORLD_BOUNDS;
		renPar.z = 0.0f;
		renPar.u = 1.0f;
		renPar.waveHeight = 0.0f;
		renPar.velocity = 0.0f;
	}
	if (y > MIN_WORLD_BOUNDS)
	{
		fy = y;
	}
	else
	{
		fy = MIN_WORLD_BOUNDS;
		y = MIN_WORLD_BOUNDS;
		renPar.z = 0.0f;
		renPar.u = 1.0f;
		renPar.waveHeight = 0.0f;
		renPar.velocity = 0.0f;
	}
	if (y >= MAX_WORLD_BOUNDS)
	{
		fy = MAX_WORLD_BOUNDS;
		renPar.z = 0.0;
		renPar.u = 1.0;
		renPar.waveHeight = 0;
		renPar.velocity = 0;
	}
	for (size_t i = 0; i < m_nNumOfWaterVertices; i++)
	{
		if (m_aVertices[i].x == fx && m_aVertices[i].y == fy && m_aVertices[i].renPar.z == renPar.z)
		{
			return i;
		}
	}
	m_aVertices[m_nNumOfWaterVertices].x = fx;
	m_aVertices[m_nNumOfWaterVertices].y = fy;
	m_aVertices[m_nNumOfWaterVertices].renPar = renPar;
	m_aVertices[m_nNumOfWaterVertices].renPar.waveHeight = 0.0f;
	return m_nNumOfWaterVertices++;
}