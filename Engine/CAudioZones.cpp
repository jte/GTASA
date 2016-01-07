#include "StdInc.h"

void CAudioZones::Init()
{
	m_NumSpheres = 0;
	m_NumBoxes = 0;
	m_NumActiveSpheres = 0;
	m_NumActiveBoxes = 0;
}

void CAudioZones::RegisterAudioSphere(char* name, int soundID, bool forcedOnOff, float centerX, float centerY, float centerZ, float radius)
{
	strcpy(m_aSpheres[m_NumSpheres].name, name);
	m_aSpheres[m_NumSpheres].soundID = soundID;
	m_aSpheres[m_NumSpheres].forcedOnOff = forcedOnOff;
	m_aSpheres[m_NumSpheres].centerX = centerX;
	m_aSpheres[m_NumSpheres].centerY = centerY;
	m_aSpheres[m_NumSpheres].centerZ = centerZ;
	m_aSpheres[m_NumSpheres].radius = radius;
	m_NumSpheres++;
}

void CAudioZones::RegisterAudioBox(char* name, int soundID, bool forcedOnOff, float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	strcpy(m_aBoxes[m_NumBoxes].name, name);
	m_aBoxes[m_NumBoxes].soundID = soundID;
	m_aBoxes[m_NumBoxes].forcedOnOff = forcedOnOff;
	m_aBoxes[m_NumBoxes].minX = FloatToFixedPoint<8>(minX);
	m_aBoxes[m_NumBoxes].minY = FloatToFixedPoint<8>(minY);
	m_aBoxes[m_NumBoxes].minZ = FloatToFixedPoint<8>(minZ);
	m_aBoxes[m_NumBoxes].maxX = FloatToFixedPoint<8>(maxX);
	m_aBoxes[m_NumBoxes].maxY = FloatToFixedPoint<8>(maxY);
	m_aBoxes[m_NumBoxes].maxZ = FloatToFixedPoint<8>(maxZ);
	m_NumBoxes++;
}

void CAudioZones::SwitchAudioZone(char* name, bool enable)
{
	for (size_t i = 0; i < m_NumSpheres; i++)
	{
		if (!stricmp(name, m_aSpheres[i].name))
		{
			m_aSpheres[i].forcedOnOff = enable;
		}
	}

	for (size_t i = 0; i < m_NumBoxes; i++)
	{
		if (!stricmp(name, m_aBoxes[i].name))
		{
			m_aBoxes[i].forcedOnOff = enable;
		}
	}
}