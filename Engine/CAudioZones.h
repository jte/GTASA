#pragma once

struct SAudioBox
{
	char name[8];
	uint16_t soundID;
	uint16_t forcedOnOff;
	FixedPoint minX, minY, minZ; // FixedPoint<8>
	FixedPoint maxX, maxY, maxZ; // FixedPoint<8>
};

struct SAudioSphere
{
	char name[8];
	uint16_t soundID;
	uint16_t forcedOnOff;
	float centerX, centerY, centerZ;
	float radius;
};

class CAudioZones
{
public:
	static void Init();
	static void RegisterAudioSphere(char* name, int soundID, bool forcedOnOff, float centerX, float centerY, float centerZ, float radius);
	static void RegisterAudioBox(char* name, int soundID, bool forcedOnOff, float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	static void SwitchAudioZone(char* name, bool enable);
	/* Update(bool, CVector)
	*/
private:
	static uint32_t m_NumActiveBoxes;
	static uint32_t m_NumActiveSpheres;
	static uint32_t m_NumBoxes;
	static uint32_t m_NumSpheres;
	static SAudioBox** m_aActiveBoxes;
	static SAudioSphere** m_aActiveSpheres;
	static SAudioBox m_aBoxes[158];
	static SAudioSphere m_aSpheres[3];
	static bool m_bRenderAudioZones;
};