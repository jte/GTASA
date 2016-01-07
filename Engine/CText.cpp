#include "StdInc.h"

static char GxtErrorString[32]; 

unsigned short* CText::Get(const char* key)
{
    if (*key != ' ' && *key != '\0')
    {
        uint8_t found = 0;
        uint16_t* text = m_keyMain.Search(key, &found);
        if (found)
        {
            return text;
        }
        if (m_missionLoaded && (CGame::IsMissionPackGame() || m_gotTextOffsets))
        {
            text = m_keyMission.Search(key, &found);
            if (found)
            {
                return text;
            }
        }
    }
    char string[24];
    sprintf(string, "");
    memcpy(GxtErrorString, string, sizeof(string));
    GxtErrorString[24] = '\0';
    return (unsigned short*)GxtErrorString;
}

void CText::Load(unsigned char a2)
{
    m_gotTextOffsets = 0;
    Unload(a2);
    CFileMgr::SetDir("TEXT");
    char* languageFileName = NULL;
    switch (FrontEndMenuManager.GetLanguage())
    {
        case 0:
            languageFileName = "AMERICAN.GXT";
        case 1:
            languageFileName = "FRENCH.GXT";
        case 2:
            languageFileName = "GERMAN.GXT";
        case 3:
            languageFileName = "ITALIAN.GXT";
        case 4:
            languageFileName = "SPANISH.GXT";
        break;
        default:
            break;
    }
    int file = CFileMgr::OpenFile(languageFileName, "rb");
    CFileMgr::Read(file, &buf, 2);
    CFileMgr::Read(file, &buf, 2);
    bool foundKey = false;
    bool foundData = false;
    uint32_t offset = 4;
    while (!foundKey || !foundData)
    {
        for (size_t i = 0; i < 8; i++)
        {
            if (CFileMgr::Read(file, &v20, 1) != 1)
            {
                break;
            }
            offset++;
            v24[i] = v20;
        }
        if (tmp)
        {
            if (!strncmp(v24, "TABL", 4))
            {
                m_missionTextOffsets.Load(tmp, file, &offset, 0x58000);
                m_gotTextOffsets = true;
            }
            else if (!strncmp(v24, "TKEY", 4))
            {
                m_keyMain.Load(tmp, file, &offset, false);
                foundKey = 1;
            }
            else if (!strncmp(v24, "TDAT", 4))
            {
                m_dataMain.Load(tmp, file, &offset, false);
                foundData = 1;
            }
            else
            {
                for (size_t i = 0; i < tmp; i++)
                {
                    CFileMgr::Read(file, &v21, 1);
                    offset++;
                }
            }
        }
    }
    m_keyMain.Update((uint16_t*)m_dataMain.GetArray());
    CFileMgr::CloseFile(file);
    unsigned short* gxtCdError = TheText.Get("CDERROR");
    const char* asciiCdError = GxtCharToAscii(gxtCdError, 0);
    strcpy(m_cdErrorText, asciiCdError);
    m_cdErrorInitialized = 1;
    CFileMgr::SetDir("");
    if (!a2 && CGame::IsMissionPackGame() && m_missionLoaded && FrontEndMenuManager.CheckMissionPackValidMenu())
    {
        CTimer::Suspend();
        LoadMissionPackText();
        CFileMgr::SetDir("");
        CTimer::Resume();
    }
}

CText::CText()
{
    m_missionLoaded = false;
    m_gotTextOffsets = false;
    m_codepage = 101;
    memset(m_missionName, 0, sizeof(m_missionName));
    memset(GxtErrorString, 0, sizeof(GxtErrorString));
}

void CText::GetNameOfLoadedMissionText(char* missionName)
{
    strcpy(missionName, m_missionName);
}

static uint8_t gTextToUpperCase[] =
{
0x80,0x81,0x82,0x83,0x84,0x85,0x86,
0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,
0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
0x95,0x96,0x80,0x81,0x82,0x83,0x84,
0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,
0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,
0x93,0x94,0x95,0xad,0xad,0xaf,0xb0,
0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,
0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,
0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,
0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,
0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,
0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,
0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,
0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,
0xfe,0xff,0x80,0x81,0x82,0x83,0x84,
0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,
0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,
0x93,0x94,0x95,0x96,0x41,0x41,0x41,
0x41,0x84,0x85,0x45,0x45,0x45,0x45,
0x49,0x49,0x49,0x49,0x4f,0x4f,0x4f,
0x4f,0x55,0x55,0x55,0x55,0xad,0xad,
0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,
0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,
0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,
0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,
0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,
0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,
0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,
0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,
0xee,0xef,0xf0,0xf1,0xf2,0xf3,0xf4,
0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,
0xfc,0xfd,0xfe,0xff
};

uint16_t CText::GetUpperCase(uint16_t ch)
{
    switch (m_codepage)
    {
        case 101:
            if (ch >= 'a' && ch <= 'z')
            {
                return ch - 32;
            }
        break;
        case 102:
            if (ch >= 'a' && ch <= 'z')
            {
                return ch - 32;
            }
            else if (ch >= 128 && ch <= 255)
            {
                return gTextToUpperCase[ch];
            }
        break;
        case 103:
        case 105:
        case 115:
            if (ch >= 'a' && ch <= 'z')
            {
                return ch - 32;
            }
            else if (ch >= 128 && ch <= 255)
            {
                return gTextToUpperCase[ch - 128];
            }
        break;
        default:
            return ch;
    }
}