#pragma once

class CFileMgr
{
public:
	//* ChangeDir(char const*)
    bool CloseFile(FILE *pFile);
	//* GetCdError()
	//* GetCdErrorString(int)
	//* GetCdFile(char const*, unsigned int&, unsigned int&)
	//* GetErrorReadWrite(int)
	size_t GetTotalSize(FILE *pFile);
	//* InitCd()
	//* InitCdSystem()
	//* Initialise()
	int LoadFile(char const *szFilename, unsigned char *pBuffer, int iBufferSize, char const *szMode);
	FILE *OpenFile(char const *szName, char const *szMode);
	FILE *OpenFileForAppending(char const *szName);
	FILE *OpenFileForWriting(char const *szName);
	size_t Read(FILE *pFile, char *pBuffer, int iBufferSize);
	//* ReadCd(unsigned int, char*, unsigned int)
	bool ReadLine(FILE *pFile, char *pBuffer, int iBufferSize);
	bool Seek(FILE *pFile, int iPos, int iFrom);
	void SetDir(char const *szDirectory);
	//* SetDirMyDocuments()
//	* SyncCd()
	size_t Tell(FILE *pFile);
	size_t Write(FILE *pFile, char *pBuffer, int iBufferSize);
private:
    static char fileDataArea[MAX_PATH];
	static char ms_dirName[MAX_PATH];
	//static bool ms_dualLayer;
	//* ms_layerStart
	static char ms_rootDirName[MAX_PATH];
};