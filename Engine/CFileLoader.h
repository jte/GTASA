#pragma once

class CFileLoader
{
public:
	//* AddTexDictionaries(RwTexDictionary*, RwTexDictionary*)
	RpAtomic *FindRelatedModelInfoCB(RpAtomic *atomic, void *data);
	//* FinishLoadClumpFile(RwStream*, unsigned int)
	//* Load2dEffect(char const*)
	//* LoadAnimatedClumpObject(char const*)
	//* LoadAtomicFile(RwStream*, unsigned int)
	void LoadAtomicFile(char const *filename);
	//* LoadAtomicFile2Return(char const*)
	//* LoadAudioZone(char const*)
	//* LoadBoundingBox(unsigned char*, CBoundingBox&)
	//* LoadCarGenerator(CFileCarGenerator*, int)
	//* LoadCarGenerator(char const*, int)
	//* LoadCarPathNode(char const*, int, int, bool)
	//* LoadClumpFile(RwStream*, unsigned int)
	//* LoadClumpFile(char const*)
	//* LoadClumpObject(char const*)
	//* LoadCollisionFile(char const*, unsigned char)
	//* LoadCollisionFile(unsigned char*, unsigned int, unsigned char)
	//* LoadCollisionFileFirstTime(unsigned char*, unsigned int, unsigned char)
	//* LoadCollisionModel(unsigned char*, CColModel&, char const*)
	//* LoadCollisionModelVer2(unsigned char*, unsigned int, CColModel&, char const*)
	//* LoadCollisionModelVer3(unsigned char*, unsigned int, CColModel&, char const*)
	//* LoadCollisionModelVer4(unsigned char*, unsigned int, CColModel&, char const*)
	//* LoadCullZone(char const*)
	//* LoadEntryExit(char const*)
	//* LoadGarage(char const*)
	//* LoadLevel(char const*)
	static const char* LoadLine(int fp);
	//* LoadLine(unsigned char**, int&)
	//* LoadObject(char const*)
	//* LoadObjectInstance(CFileObjectInstance*, char const*)
	//* LoadObjectInstance(char const*)
	//* LoadObjectTypes(char const*)
	//* LoadOcclusionVolume(char const*, char const*)
	//* LoadPathHeader(char const*, int&)
	//* LoadPedObject(char const*)
	//* LoadPedPathNode(char const*, int, int)
	//* LoadPickup(char const*)
	//* LoadScene(char const*)
	//* LoadStuntJump(char const*)
	//* LoadTXDParent(char const*)
	//* LoadTexDictionary(char const*)
	//* LoadTimeCyclesModifier(char const*)
	//* LoadTimeObject(char const*)
	//* LoadVehicleObject(char const*)
	//* LoadWeaponObject(char const*)
	//* LoadZone(char const*)
	//* ReloadObjectTypes(char const*)
	//* ReloadPaths(char const*)
	//* SaveTexDictionary(RwTexDictionary*, char const*)
	//* SetRelatedModelInfoCB(RpAtomic*, void*)
	//* StartLoadClumpFile(RwStream*, unsigned int)
protected:
private:
    static char ms_line[512];
};