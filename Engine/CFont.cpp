#include "StdInc.h"

void CFont::LoadFontValues()
{
    CFileMgr::SetDir("");
    int fp = (int)CFileMgr::OpenFile("DATA\\FONTS.DAT", "rb");
    size_t totalFonts = 0;
    size_t fontId = 0;
    for(const char* line = CFileLoader::LoadLine(fp); line; line = CFileLoader::LoadLine(fp))
    {
        if(line[0] == '#' || line[0] == '\0')
        {
            continue;
        }
        char buffer[26+1];
        sscanf(line, "%s", buffer);
        if(!strcmp(buffer, "[TOTAL_FONTS]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", &totalFonts);
        }
        else if(!strcmp(buffer, "[FONT_ID]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", &fontId);
        }
        else if(!strcmp(buffer, "[REPLACEMENT_SPACE_CHAR]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", &Size[fontId].replacementSpaceChar);
        }
        else if(!strcmp(buffer, "[PROP]"))
        {
            line = CFileLoader::LoadLine(fp);
            size_t p[8];
            sscanf(line, "%d %d %d %d %d %d %d %d", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            for(size_t i = 0; i < ELEMS_COUNT(p); i++)
            {
                Size[fontId].proportionalValues[i] = p[i];
            }
        }
        else if(!strcmp(buffer, "[UNPROP]"))
        {
            line = CFileLoader::LoadLine(fp);
            sscanf(line, "%d", Size[fontId].unproportionalValue);
        }
    }
    CFileMgr::CloseFile((FILE*)fp);
}

uint8_t* CFont::ParseToken(unsigned short* string, CRGBA& color, unsigned char a3, char* a4)
{
    uint8_t code = *(uint8_t*)(string + 1);
    switch(code)
    {
    case 'R':
    case 'r':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(0, color.alpha);
    break;
    case 'G':
    case 'g':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(1, color.alpha);
    break;
    case 'B':
    case 'b':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(2, color.alpha);
    break;
    case 'W':
    case 'w':
        if(a3 && a4)
        {
            *a4 = code;
            break;
        }
        color = CHudColours::GetRGB(4, color.alpha); // GTA bug: should be "3" for WHITE
    break;
    case 'H':
    case 'h':
        if(a3 && a4)
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
    if(*lptr != '~')
    {
        while(*(lptr + 1) != '~')
        {
            lptr++;
        }
    }
    if(*lptr)
    {
        return (lptr + 1);
    }
    else
    {
        return (lptr + 2);
    }
}