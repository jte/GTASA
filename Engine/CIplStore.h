#pragma once

#include "CPool.h"
#include "CQuadTreeNode.h"

struct IplDef
{ // sizeof() == 52
    float bbox[4];
    char* name;

    bool isInterior;
};
/*
00000000 bbox            dd 4 dup(?)
00000010 m_name          dd ?
00000014 field_14        dd ?
00000018 field_18        dd ?
0000001C field_1C        dd ?
00000020 field_20        dd ?
00000024 field_24        dd ?
00000028 field_28        db ?
00000029 field_29        db ?
0000002A m_instanceBlockId dw ?
0000002C m_isInterior    db ?
0000002D field_2D        db ?
0000002E m_waitingToBeLoaded db ?
0000002F m_dynamicStreamingDisabled db ?
00000030 field_30        db ?
00000031 field_31        db ?
00000032 field_32        db ?
00000033 field_33        db ?
*/


class CIplStore
{
public:
    const char* GetIplName(size_t id);
    void SetIsInterior(size_t id, bool isInterior);
    size_t FindIplSlot(const char* name);
 /*   * AddIplSlot(char const*)
	* AddIplsNeededAtPosn(CVector const&)
	* ClearIplsNeededAtPosn()
	* EnableDynamicStreaming(int, bool)
	* EnsureIplsAreInMemory(CVector const&)
	* FindRelatedIplSlot(char const*)
	* GetBoundingBox(int)
	* GetIplEntityIndexArray(int)
	* GetIplName(int)
	* GetNewIplEntityIndexArray(int)
	* HaveIplsLoaded(CVector const&, int)
	* IncludeEntity(int, CEntity*)
	* Initialise()
	* Load()
	* LoadAllRemainingIpls()
	* LoadIpl(int, unsigned char*, int)
	* LoadIplBoundingBox(int, unsigned char*, int)
	* LoadIpls(CVector, bool)
	* RemoveAllIpls()
	* RemoveIpl(int)
	* RemoveIplAndIgnore(int)
	* RemoveIplSlot(int)
	* RemoveIplWhenFarAway(int)
	* RemoveRelatedIpls(int)
	* RequestIplAndIgnore(int)
	* RequestIpls(CVector const&, int)
	* Save()
	* SetIplsRequired(CVector const&, int)
	* SetupRelatedIpls(char const*, int, CEntity**)
	* Shutdown()*/
private:
    static size_t m_onlyBB;
	static CPool<IplDef> ms_pPool;
	static CQuadTreeNode* ms_pQuadTree;
};