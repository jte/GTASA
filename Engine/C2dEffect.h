#pragma once

typedef int CRoadsignAttrFlags;

class C2dEffect
{
public:
	static bool PluginAttach();
	size_t Roadsign_GetNumLettersFromFlags(CRoadsignAttrFlags flags);
	size_t Roadsign_GetNumLinesFromFlags(CRoadsignAttrFlags flags);
	size_t Roadsign_GetPaletteIDFromFlags(CRoadsignAttrFlags flags);
	void Shutdown();
protected:
private:
};