#pragma once

class CFileMgr
{
public:
	//* ChangeDir(char const*)
    static bool CloseFile(int file);
	//* GetCdError()
	//* GetCdErrorString(int)
	//* GetCdFile(char const*, unsigned int&, unsigned int&)
	//* GetErrorReadWrite(int)
	static size_t GetTotalSize(int file);
	//* InitCd()
	//* InitCdSystem()
	static void Initialise();
	static int LoadFile(char const* filename, unsigned char* buffer, int bufferSize, char const* mode);
	static int OpenFile(char const* filename, char const* mode);
	static int OpenFileForAppending(char const* filename);
	static int OpenFileForWriting(char const* filename);
	static size_t Read(int file, char* buffer, int bufferSize);
	//* ReadCd(unsigned int, char*, unsigned int)
	static bool ReadLine(int file, char* buffer, int bufferSize);
	static bool Seek(int file, int pos, int from);
	static void SetDir(char const* directory);
	//* SetDirMyDocuments()
//	* SyncCd()
	static size_t Tell(int file);
	static size_t Write(int file, char* buffer, int bufferSize);
private:
    static char fileDataArea[MAX_PATH];
	static char ms_dirName[MAX_PATH];
	//static bool ms_dualLayer;
	//* ms_layerStart
	static char ms_rootDirName[MAX_PATH];
};