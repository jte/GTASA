#pragma once

class CRadar
{
public:
	/*

		* AddBlipToLegendList(unsigned char, int)
	* CalculateBlipAlpha(float)
	* CalculateCachedSinCos()
	* ChangeBlipBrightness(int, int)
	* ChangeBlipColour(int, unsigned int)
	* ChangeBlipDisplay(int, eBlipDisplay)
	* ChangeBlipScale(int, int)
	* ClearActualBlip(int)
	* ClearBlip(int)
	* ClearBlipForEntity(eBlipType, int)
	* ClipRadarPoly(CVector2D*, CVector2D const*)
	* DisplayThisBlip(int, signed char)
	* Draw3dMarkers()
	* DrawAreaOnRadar(CRect const&, CRGBA const&, bool)
	* DrawBlips(float)
	* DrawCoordBlip(int, unsigned char, int, float)
	* DrawEntityBlip(int, unsigned char, int, float)
	* DrawLegend(int, int, int)
	* DrawMap()
	* DrawRadarGangOverlay(bool)
	* DrawRadarMap()
	* DrawRadarMask()
	* DrawRadarSection(int, int, int)
	* DrawRadarSectionMap(int, int, CRect, float)
	* DrawRadarSprite(unsigned short, float, float, unsigned char)
	* DrawRotatingRadarSprite(CSprite2d*, float, float, float, float, CRGBA)
	* DrawYouAreHereSprite(float, float)
	* GetActualBlipArrayIndex(int)
	* GetNewUniqueBlipIndex(int)
	* GetRadarTraceColour(unsigned int, unsigned char, unsigned char)
	* HasThisBlipBeenRevealed(int)
	* InitFrontEndMap()
	* Initialise()
	* LimitRadarPoint(CVector2D&)
	* LimitToMap(float*, float*)
	* Load()
	* LoadTextures()
	* RemoveMapSection(int, int)
	* RemoveRadarSections()
	* RequestMapSection(int, int)
	* Save()
	* SetBlipAlwaysDisplayInZoom(int, unsigned char)
	* SetBlipEntryExit(int, CEntryExit*)
	* SetBlipFade(int, unsigned char)
	* SetBlipFriendly(int, unsigned char)
	* SetBlipSprite(int, int)
	* SetCoordBlip(eBlipType, CVector, unsigned int, eBlipDisplay, char*)
	* SetCoordBlipAppearance(int, unsigned char)
	* SetEntityBlip(eBlipType, int, unsigned int, eBlipDisplay, char*)
	* SetMapCentreToPlayerCoords()
	* SetRadarMarkerState(int, unsigned char)
	* SetShortRangeCoordBlip(eBlipType, CVector, unsigned int, eBlipDisplay, char*)
	* SetupAirstripBlips()
	* SetupRadarRect(int, int)
	* ShowRadarMarker(CVector, unsigned int, float)
	* ShowRadarTrace(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char)
	* ShowRadarTraceWithHeight(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
	* Shutdown()
	* StreamRadarSections(CVector const&)
	* StreamRadarSections(int, int)
	* TransformRadarPointToRealWorldSpace(CVector2D&, CVector2D const&)
	* TransformRadarPointToScreenSpace(CVector2D&, CVector2D const&)
	* TransformRealWorldPointToRadarSpace(CVector2D&, CVector2D const&)
	* TransformRealWorldToTexCoordSpace(CVector2D&, CVector2D const&, int, int)
	* TransformScreenSpaceToRadarPoint(CVector2D&, CVector2D&)
	*/
private:
	/*
		* ArrowBlipColour
	* MapLegendCounter
	* MapLegendList
	* RadarBlipSprites
	* airstrip_blip
	* airstrip_location
	* m_radarRange
	* m_radarRect
	* ms_RadarTrace
	* vec2DRadarOrigin*/
};