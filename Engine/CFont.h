#pragma once

#include "CSprite2d.h"

class CFontDetails
{
public:
    CRGBA color;
    float scaleX;
    float scaleY;
    float slant;
    float slantRefX;
    float slantRefY;
    uint8_t justify;
    uint8_t useCentreSize;
    uint8_t useRightJustify;
    uint8_t backgroundX;
    uint8_t backgroundY;
    uint8_t proportional;
    uint8_t saveTokenColor;
    uint8_t pad4;
    float alphaFade;
    CRGBA backgroundColor;
    float wrapX;
    float centreSize;
    float rightJustifyWrap;
    uint8_t fontId;
    uint8_t pad5;
    uint8_t dropShadowPos;
    CRGBA dropColor;
    uint8_t pad7;
    uint8_t pad8;
};

class CFontRenderState
{
public:

};

class CFont
{
public:
	//* AddEFIGSFont()
	//* AddJapaneseTexture()
	//* AddRussianTexture()
	//* AddStandardTexture()
	//* ClearSpecialLanguages()
	//* ComputeHeight(float)
	//* FilterOutTokensFromString(unsigned short*)
	//* FindSubFontCharacter(unsigned short, unsigned char)
	//* GetCharacterSize(unsigned short)
	//* GetHeight(bool)
	//* GetNextSpace(unsigned short*)
	static size_t GetNumberLines(float x, float y, uint16_t* text);
	static size_t GetStringWidth(unsigned short*, unsigned char, unsigned char);
	//* GetStringWidth(unsigned short*, unsigned char, unsigned char, unsigned char)
	static void GetTextRect(CRect* textRect, float x, float y, uint16_t* text);
	static void InitPerFrame();
	static void Initialise();
	static void LoadFontValues();
	static uint8_t* ParseToken(unsigned short* string, CRGBA& color, unsigned char saveColor, char* colorToken);
	//* PrintChar(float, float, unsigned short)
	//* PrintString(float, float, unsigned short*)
	//* PrintStringFromBottom(float, float, unsigned short*)
	//* ProcessCurrentString(unsigned char, float, float, unsigned short*)
	//* ProcessStringToDisplay(float, float, unsigned short*)
	//* RemapCharacterWithoutGraphic(unsigned short)
	//* RenderFontBuffer()
	//* RenderString(float, float, unsigned short*, unsigned short*, float)
	static void SetAlphaFade(float alphaFade);
	static void SetBackground(uint8_t backgroundX, uint8_t backgroundY);
	static void SetBackgroundColor(CRGBA color);
	static void SetCentreSize(float centreSize);
	static void SetColor(CRGBA color);
	static void SetDropColor(CRGBA color);
	// SetDropShadowPosition(signed char)
	//* SetEdge(signed char)
	//* SetFontStyle(unsigned char)
	static void SetJustify(uint8_t justify);
	static void SetOrientation(uint8_t orientation);
	static void SetProportional(uint8_t proportional);
	static void SetRightJustifyWrap(float rightJustifyWrap);
	static void SetScale(float scaleX, float scaleY);
	static void SetSlant(float);
	static void SetSlantRefPoint(float slantRefX, float slantRefY);
	static void SetWrapx(float wrapX);
	static void Shutdown();
	//* character_code(unsigned char)
private:
	static CFontDetails Details;
	static bool NewLine;
	static CSprite2d PS2Sprite[15];
	static size_t PS2Symbol;
	static CFontRenderState RenderState;
	static struct 
    {
        char replacementSpaceChar;
        size_t proportionalValues[8];
        size_t unproportionalValue;
    } Size[256/*a guess; needs research*/];
	static CSprite2d Sprite[2];
	//* UsingEFIGSLanguage
	//* UsingJapaneseLanguage
	//* UsingKoreanLanguage
	//* UsingRussianLanguage
};