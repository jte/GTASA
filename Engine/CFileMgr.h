#pragma once

class CFileMgr
{
public:
	//* ChangeDir(char const*)
    static bool CloseFile(FILE *pFile);
	//* GetCdError()
	//* GetCdErrorString(int)
	//* GetCdFile(char const*, unsigned int&, unsigned int&)
	//* GetErrorReadWrite(int)
	static size_t GetTotalSize(FILE *pFile);
	//* InitCd()
	//* InitCdSystem()
	//* Initialise()
	static int LoadFile(char const *szFilename, unsigned char *pBuffer, int iBufferSize, char const *szMode);
	static FILE *OpenFile(char const *szName, char const *szMode);
	static FILE *OpenFileForAppending(char const *szName);
	static FILE *OpenFileForWriting(char const *szName);
	static size_t Read(FILE *pFile, char *pBuffer, int iBufferSize);
	//* ReadCd(unsigned int, char*, unsigned int)
	static bool ReadLine(FILE *pFile, char *pBuffer, int iBufferSize);
	static bool Seek(FILE *pFile, int iPos, int iFrom);
	static void SetDir(char const *szDirectory);
	//* SetDirMyDocuments()
//	* SyncCd()
	static size_t Tell(FILE *pFile);
	static size_t Write(FILE *pFile, char *pBuffer, int iBufferSize);
private:
    static char fileDataArea[MAX_PATH];
	static char ms_dirName[MAX_PATH];
	//static bool ms_dualLayer;
	//* ms_layerStart
	static char ms_rootDirName[MAX_PATH];
};