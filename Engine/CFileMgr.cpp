#include "StdInc.h"

void CFileMgr::Initialise()
{
    getcwd(fileDataArea);
    strcat(fileDataArea, "\\");
}

void CFileMgr::SetDir(char const *szDirectory)
{
    strcpy(ms_dirName, fileDataArea);
    if(szDirectory && szDirectory[0])
    {
        strcat(ms_dirName, szDirectory);
        if(szDirectory[strlen(szDirectory) - 1] != '\\')
        {
            strcat(ms_dirName, "\\");
        }
    }
    return _chdir(ms_dirName);
}

int CFileMgr::LoadFile(char const *szFilename, unsigned char *pBuffer, int iBufferSize, char const *szMode)
{
    FILE *pFile = OpenFile(szFilename, szMode);
    if(pFile == NULL)
    {
        return -1;
    }
    size_t uiBytesRead = Read(pFile, pBuffer, iBufferSize);
    if(uiBytesRead == iBufferSize)
    {
        pBuffer[uiBytesRead] = '\0';
        CloseFile();
        return uiBytesRead;
    }
    CloseFile();
    return -1;
}

FILE *CFileMgr::OpenFile(char const *szName, char const *szMode)
{
    return fopen(szName, szMode);
}

FILE *CFileMgr::OpenFileForWriting(char const *szName)
{
    return OpenFile(szName, "wb");
}

FILE *CFileMgr::OpenFileForAppending(char const *szName)
{
    return OpenFile(szName, "a");
}

size_t CFileMgr::Read(FILE *pFile, char *pBuffer, int iBufferSize)
{
    return fread(pBuffer, 1, iBufferSize, pFile);
}

size_t CFileMgr::Write(FILE *pFile, char *pBuffer, int iBufferSize)
{
    return fwrite(pBuffer, 1, iBufferSize, pFile);
}

bool CFileMgr::Seek(FILE *pFile, int iPos, int iFrom)
{
    return fseek(pFile, iPos, iFrom) != 0;
}

bool CFileMgr::ReadLine(FILE *pFile, char *pBuffer, int iBufferSize)
{
    return fgets(pBuffer, iBufferSize, pFile) != 0;
}

bool CFileMgr::CloseFile(FILE *pFile)
{
    return fclose(pFile);
}

size_t CFileMgr::GetTotalSize(FILE *pFile)
{
    size_t uiRestore = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    size_t uiEnd = ftell(pFile);
    fseek(pFile, uiRestore, SEEK_SET);
    return uiEnd;
}

size_t CFileMgr::Tell(FILE *pFile)
{
    return ftell(pFile);
}