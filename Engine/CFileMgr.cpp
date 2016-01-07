#include "StdInc.h"

void CFileMgr::Initialise()
{
    _getcwd(fileDataArea, sizeof(fileDataArea));
    strcat(fileDataArea, "\\");
}

void CFileMgr::SetDir(char const* directory)
{
    strcpy(ms_dirName, fileDataArea);
    if (directory && directory[0])
    {
        strcat(ms_dirName, directory);
        if(directory[strlen(directory) - 1] != '\\')
        {
            strcat(ms_dirName, "\\");
        }
    }
    _chdir((const char*)ms_dirName);
}

int CFileMgr::LoadFile(char const* filename, unsigned char* buffer, int bufferSize, char const* mode)
{
    int file = OpenFile(filename, mode);
    if (file == NULL)
    {
        return -1;
    }
    size_t bytesRead = Read(file, (char*)buffer, bufferSize);
    if (bytesRead == bufferSize)
    {
        buffer[bytesRead] = '\0';
        CloseFile(file);
        return bytesRead;
    }
    CloseFile(file);
    return -1;
}

int CFileMgr::OpenFile(char const* name, char const* mode)
{
    return (int)fopen(name, mode);
}

int CFileMgr::OpenFileForWriting(char const* name)
{
    return OpenFile(name, "wb");
}

int CFileMgr::OpenFileForAppending(char const* name)
{
    return OpenFile(name, "a");
}

size_t CFileMgr::Read(int file, char* buffer, int bufferSize)
{
    return fread(buffer, 1, bufferSize, (FILE*)file);
}

size_t CFileMgr::Write(int file, char* buffer, int bufferSize)
{
    return fwrite(buffer, 1, bufferSize, (FILE*)file);
}

bool CFileMgr::Seek(int file, int pos, int from)
{
    return fseek((FILE*)file, pos, from) != 0;
}

bool CFileMgr::ReadLine(int file, char* buffer, int bufferSize)
{
    return fgets(buffer, bufferSize, (FILE*)file) != 0;
}

bool CFileMgr::CloseFile(int file)
{
    return fclose((FILE*)file);
}

size_t CFileMgr::GetTotalSize(int file)
{
    size_t restore = ftell((FILE*)file);
    fseek((FILE*)file, 0, SEEK_END);
    size_t end = ftell((FILE*)file);
    fseek((FILE*)file, restore, SEEK_SET);
    return end;
}

size_t CFileMgr::Tell(int file)
{
    return ftell((FILE*)file);
}

static char gUserPath[MAX_PATH];

static char* InitUserDirectories()
{
    /* Find path to My Documents directory */
    HKEY hKey = NULL;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 0, KEY_READ, &hKey))
    {
        strcpy(gUserPath, "data");
        return gUserPath;
    }
    DWORD cbData = 256;
    if (RegQueryValueExA(hKey, "Personal", 0, &Type, (LPBYTE)gUserPath, &cbData))
    {
        RegCloseKey(hKey);
        strcpy(gUserPath, "data");
        return gUserPath;
    }
    RegCloseKey(hKey);
    strcat(gUserPath, "\\GTA San Andreas User Files");
    HANDLE hUserFiles = CreateFileA(gUserPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0x2000080u, 0);
    if (hUserFiles == INVALID_HANDLE_VALUE)
    {
        CreateDirectoryA(gUserPath, 0);
    }
    else
    {
        CloseHandle(hUserFiles);
    }

    char galleryPath[MAX_PATH];
    strcpy(galleryPath, gUserPath);
    strcat(galleryPath, "\\Gallery");
    HANDLE hGalleryFiles = CreateFileA(galleryPath, 0x80000000u, 1u, 0, 3u, 0x2000080u, 0);
    if (hGalleryFiles == INVALID_HANDLE_VALUE)
    {
        CreateDirectoryA(galleryPath, 0);
    }
    else
    {
        CloseHandle(v9);
    }

    char userTracksPath[MAX_PATH];
    strcpy(userTracksPath, gUserPath);
    strcat(userTracksPath, "\\User Tracks");
    HANDLE hUserTracksFiles = CreateFileA(userTracksPath, 0x80000000u, 1u, 0, 3u, 0x2000080u, 0);
    if (hUserTracksFiles == INVALID_HANDLE_VALUE)
    {
        CreateDirectoryA(userTracksPath, 0);
    }
    else
    {
        CloseHandle(hUserTracksFiles);
    }
    return gUserPath;
}