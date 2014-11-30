#pragma once

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
	//* GetNumberLines(float, float, unsigned short*)
	//* GetStringWidth(unsigned short*, unsigned char, unsigned char)
	//* GetStringWidth(unsigned short*, unsigned char, unsigned char, unsigned char)
	//* GetTextRect(CRect*, float, float, unsigned short*)
	//* InitPerFrame()
	//* Initialise()
	void LoadFontValues();
	uint8_t* ParseToken(unsigned short* string, CRGBA& color, unsigned char saveColor, char* colorToken);
	//* PrintChar(float, float, unsigned short)
	//* PrintString(float, float, unsigned short*)
	//* PrintStringFromBottom(float, float, unsigned short*)
	//* ProcessCurrentString(unsigned char, float, float, unsigned short*)
	//* ProcessStringToDisplay(float, float, unsigned short*)
	//* RemapCharacterWithoutGraphic(unsigned short)
	//* RenderFontBuffer()
	//* RenderString(float, float, unsigned short*, unsigned short*, float)
	//* SetAlphaFade(float)
	//* SetBackground(unsigned char, unsigned char)
	//* SetBackgroundColor(CRGBA)
	//* SetCentreSize(float)
	//* SetColor(CRGBA)
	//* SetDropColor(CRGBA)
	//* SetDropShadowPosition(signed char)
	//* SetEdge(signed char)
	//* SetFontStyle(unsigned char)
	//* SetJustify(unsigned char)
	//* SetOrientation(unsigned char)
	//* SetProportional(unsigned char)
	//* SetRightJustifyWrap(float)
	//* SetScale(float)
	//* SetSlant(float)
	//* SetSlantRefPoint(float, float)
	//* SetWrapx(float)
	//* Shutdown()
	//* character_code(unsigned char)
private:
	//* Details
	static bool NewLine;
	//* PS2Sprite
	static size_t PS2Symbol;
	//* RenderState
	struct 
    {
        char replacementSpaceChar;
        size_t proportionalValues[8];
        size_t unproportionalValue;
    } Size[256/*a guess; needs research*/];
	//* Sprite
	//* UsingEFIGSLanguage
	//* UsingJapaneseLanguage
	//* UsingKoreanLanguage
	//* UsingRussianLanguage
};