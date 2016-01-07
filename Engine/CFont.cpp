#include "StdInc.h"

void CFont::LoadFontValues()
{
    CFileMgr::SetDir("");
    int fp = (int)CFileMgr::OpenFile("DATA\\FONTS.DAT", "rb");
    size_t totalFonts = 0;
    size_t fontId = 0;
    for (const char* line = CFileLoader::LoadLine(fp); line; line = CFileLoader::LoadLine(fp))
    {
        if (line[0] == '#' || line[0] == '\0')
        {
            continue;
        }
        char buffer[26+1];
        sscanf(line, "%s", buffer);
        if (!strcmp(buffer, "[TOTAL_FONTS]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", &totalFonts);
        }
        else if (!strcmp(buffer, "[FONT_ID]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", &fontId);
        }
        else if (!strcmp(buffer, "[REPLACEMENT_SPACE_CHAR]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", &Size[fontId].replacementSpaceChar);
        }
        else if (!strcmp(buffer, "[PROP]"))
        {
            line = CFileLoader::LoadLine(fp);
            size_t p[8];
            sscanf(line, "%d %d %d %d %d %d %d %d", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            for(size_t i = 0; i < ELEMS_COUNT(p); i++)
            {
                Size[fontId].proportionalValues[i] = p[i];
            }
        }
        else if (!strcmp(buffer, "[UNPROP]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", Size[fontId].unproportionalValue);
        }
    }
    CFileMgr::CloseFile(fp);
}

uint8_t* CFont::ParseToken(unsigned short* string, CRGBA& color, unsigned char a3, char* a4)
{
    uint8_t code = *(uint8_t*)(string + 1);
    switch(code)
    {
    case 'R':
    case 'r':
        if (a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(0, color.alpha);
    break;
    case 'G':
    case 'g':
        if (a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(1, color.alpha);
    break;
    case 'B':
    case 'b':
        if (a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(2, color.alpha);
    break;
    case 'W':
    case 'w':
        if (a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(4, color.alpha); // GTA bug: should be "3" for WHITE
    break;
    case 'H':
    case 'h':
        if (a3 && a4)
        {
            *a4 = code;
            break;
        }
        uint8_t r = max(color.red * 1.5f, 255);
        uint8_t g = max(color.green * 1.5f, 255);
        uint8_t b = max(color.blue * 1.5f, 255);
        color = CRGBA(r, g, b, color.alpha);
    break;
    case 'Y':
    case 'y':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(11, color.alpha);
    break;
    case 'P':
    case 'p':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(7, color.alpha);
    break;
    case 'l':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(5, color.alpha);
    break;
    case 'S':
    case 's':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(4, color.alpha);
    break;
    case 'Q':
    case 'q':
        PS2Symbol = 7;
    break;
    case 'T':
    case 't':
        PS2Symbol = 8;
    break;
    case 'O':
    case 'o':
        PS2Symbol = 6;
    break;
    case 'X':
    case 'x':
        PS2Symbol = 5;
    break;
    case '<':
        PS2Symbol = 3;
    break;
    case '>':
        PS2Symbol = 4;
    break;
    case 'U':
    case 'u':
        PS2Symbol = 1;
    break;
    case 'D':
    case 'd':
        PS2Symbol = 2;
    break;
    case 'K':
    case 'k':
        PS2Symbol = 9;
    break;
    case 'M':
    case 'm':
        PS2Symbol = 10;
    break;
    case 'A':
    case 'a':
        PS2Symbol = 11;
    break;
    case 'J':
    case 'j':
        PS2Symbol = 12;
    break;
    case 'V':
    case 'v':
        PS2Symbol = 13;
    break;
    case 'C':
    case 'c':
        PS2Symbol = 14;
    break;
    case 'N':
    case 'n':
        NewLine = true;
    break;
    default:
    break;
    }
    uint8_t* lptr = (uint8_t*)(string + 1);
    if (*lptr != '~')
    {
        while (*(lptr + 1) != '~')
        {
            lptr++;
        }
    }
    if (*lptr)
    {
        return (lptr + 1);
    }
    else
    {
        return (lptr + 2);
    }
}

void CFont::SetScale(float scaleX, float scaleY)
{
    Details.scaleX = scaleX;
    Details.scaleY = scaleY;
}

void CFont::SetSlantRefPoint(float slantRefX, float slantRefY)
{
    Details.slantRefX = slantRefX;
    Details.slantRefY = slantRefY;
}

void CFont::SetSlant(float slant)
{
    Details.slant = slant;
}

void CFont::SetColor(CRGBA color)
{
    Details.color = color;
    if (Details.alphaFade < 255.0f)
    {
        Details.color.alpha = Details.color.alpha * Details.alphaFade / 255.0f;
    }
}

void CFont::SetJustify(uint8_t justify)
{
    Details.justify = justify;
}

void CFont::SetWrapx(float wrapX)
{
    Details.wrapX = wrapX;
}

void CFont::SetCentreSize(float centreSize)
{
    Details.centreSize = centreSize;
}

void CFont::SetBackgroundColor(CRGBA color)
{
    Details.backgroundColor = color;
}

void CFont::SetProportional(uint8_t proportional)
{
    Details.proportional = proportional;
}

void CFont::SetRightJustifyWrap(float rightJustifyWrap)
{
    Details.rightJustifyWrap = rightJustifyWrap;
}

void CFont::SetBackground(uint8_t backgroundX, uint8_t backgroundY)
{
    Details.backgroundX = backgroundX;
    Details.backgroundY = backgroundY;
}

void CFont::SetAlphaFade(float alphaFade)
{
    Details.alphaFade = alphaFade;
}

void CFont::SetDropColor(CRGBA color)
{
    Details.dropColor = color;
    if (Details.alphaFade < 255.0f)
    {
        Details.dropColor.alpha = Details.color.alpha * Details.alphaFade / 255.0f;
    }
}

void CFont::Initialise()
{
    uint32_t fontTxdSlot = CTxdStore::AddTxdSlot("fonts");
    CTxdStore::LoadTxd(fontTxdSlot, "MODELS\\FONTS.TXD");
    CTxdStore::AddRef(fontTxdSlot);
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(fontTxdSlot);
    Sprite[0].SetTexture("font2", "font2m");
    Sprite[1].SetTexture("font1", "font1m");
    LoadFontValues();
    SetScale(1.0, 1.0);
    SetSlantRefPoint(RsGlobal.maximumWidth, 0);
    SetSlant(0);
    SetColor(CRGBA(255, 255, 255, 0));
    SetOrientation(1);
    SetJustify(0);
    CFont::SetWrapx(RsGlobal.maximumWidth);
    SetCentreSize(RsGlobal.maximumWidth);
    SetBackground(0, 0);
    SetBackgroundColor(CRGBA(128, 128, 128, 128));
    SetProportional(1);
    SetFontStyle(0);
    SetRightJustifyWrap(0.0);
    SetAlphaFade(255.0);
    SetDropShadowPosition(0);
    CTxdStore::PopCurrentTxd();
    uint32_t ps2btns = CTxdStore::AddTxdSlot("ps2btns");
    CTxdStore::LoadTxd(ps2btns, "MODELS\\PCBTNS.TXD");
    CTxdStore::AddRef(ps2btns);
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(ps2btns);
    PS2Sprite[1].SetTexture("up", "upA");
    PS2Sprite[2].SetTexture("down", "downA");
    PS2Sprite[3].SetTexture("left", "leftA");
    PS2Sprite[4].SetTexture("right", "rightA");
    CTxdStore::PopCurrentTxd();
}

void CFont::Shutdown()
{
    Sprite[0].Delete();
    Sprite[1].Delete();
    CTxdStore::RemoveTxdSlot(CTxdStore::FindTxdSlot("fonts"));
    for (size_t i = 1; i < 4; i++)
    {
        PS2Sprite[i].Delete();
    }
    CTxdStore::RemoveTxdSlot(CTxdStore::FindTxdSlot("ps2btns"));
}

void CFont::SetOrientation(uint8_t orientation)
{
    switch (orientation)
    {
        case 0:
            Details.useCentreSize = 1;
            Details.useRightJustify = 0;
        break;
        case 1:
            Details.useCentreSize = 0;
            Details.useRightJustify = 0;
        break;
        case 2:
            Details.useCentreSize = 0;
            Details.useRightJustify = 1;
        break;
    }
}

void CFont::GetTextRect(CRect* textRect, float x, float y, uint16_t* text)
{
    size_t numberLines = CFont::GetNumberLines(x, y, text);
    if (Details.useCentreSize)
    {
        textRect->left = x - (CFont::Details.centreSize * 0.5 + 4.0);
        textRect->right= x + (CFont::Details.centreSize * 0.5 + 4.0);
    }
    else if (Details.useRightJustify)
    {
        textRect->left = Details.rightJustifyWrap - 4.0;
        textRect->right = x + 4.0f;
    }
    else
    {
        textRect->left = x - 4.0;
        textRect->right = Details.wrapX + 4.0f;
    }
    textRect->top = y - 4.0;
    textRect->bottom = (CFont::Details.scaleY * 32.0 * 0.5 + CFont::Details.scaleY + CFont::Details.scaleY) * numberLines
                    + y
                    + 4.0;
}

size_t CFont::GetNumberLines(float x, float y, uint16_t* text)
{
    return ProcessCurrentString(false, x, y, text);
}

void CFont::InitPerFrame()
{
	Details.fixedCharSize = 0;
	Details.field_3C = 0;
	Details.dropShadowPos = 0;
	NewLine = 0;
	PS2Symbol = 0;
	RenderState.fontId = -1;
	gpFontRenderState = &gFontRenderStateDefault;
	CSprite::InitSpriteBuffer();
}

size_t CFont::GetStringWidth(unsigned short* str, unsigned char a2, unsigned char a3)
{
  char v3; // bl@1
  signed int v4; // edx@1
  signed int i; // eax@1
  signed int v6; // ecx@2
  _BYTE *v7; // esi@6
  char v8; // al@7
  char v9; // al@14
  _BYTE *v10; // esi@14
  char v11; // al@15
  char v12; // al@22
  char v14; // [sp+Bh] [bp-195h]@1
  float v15; // [sp+Ch] [bp-194h]@1
  _BYTE v16[400]; // [sp+10h] [bp-190h]@5

  v15 = 0.0;
  v14 = 0;
  v3 = 0;
  v4 = (unsigned __int16)CMessages::GetGxtStringLength(str);
  for ( i = 0; ; ++i )
  {
    v6 = 399;
    if ( v4 <= 399 )
      v6 = v4;
    if ( i >= v6 )
      break;
    v16[i] = *(&v16[str - v16] + i);
  }
  v16[i] = 0;
  CMessages::InsertPlayerControlKeysInString((int)v16);
  v7 = v16;
  while ( 1 )
  {
    v8 = *v7;
    if ( *v7 == ' ' )
    {
      if ( !a2 )
        break;
    }
    if ( !v8 )
      break;
    if ( v8 == '~' )
    {
      if ( !a2 && (v14 || v3) )
        return v15;
      v9 = v7[1];
      v10 = v7 + 1;
      if ( v9 != '~' )
      {
        do
          v11 = (v10++)[1];
        while ( v11 != '~' );
      }
      v7 = v10 + 1;
      if ( v3 || *v7 == '~' )
        v14 = 1;
    }
    else
    {
      if ( !a2 && v8 == ' ' && v14 )
        return v15;
      v12 = v8 - ' ';
      if ( a3 )
      {
        ++v7;
        v15 = sub_719670(v12) + v15;
        v3 = 1;
      }
      else
      {
        ++v7;
        v15 = CFont::GetCharacterSize(v12) + v15;
        v3 = 1;
      }
    }
  }
  return v15;
}