#pragma once

class CPlayerCrossHair
{
public:
    * Render(int)
	* Update(int, CPad*)
protected:
private:
00000000 CPlayerCrossHair struc ; (sizeof=0x2C)
00000000 bActive         db ?
00000001 field_1         db ?
00000002 field_2         db ?
00000003 field_3         db ?
00000004 uiClearTime     dd ?
00000008 vecTargetPos    RwV3D ?
00000014 red        db ?
00000015 green        db ?
00000016 blue        db ?
00000017 alpha        db ?
00000018 field_18        dd ?
0000001C field_1C        dd ?
00000020 uiScreenSize        dd ?
00000024 uiScreenScale        dd ?
00000028 field_28        db ?
00000029 field_29        db ?
0000002A field_2A        db ?
0000002B field_2B        db ?
0000002C CPlayerCrossHair ends

};