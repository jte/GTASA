/**
 */
#pragma once

class CGame
{
public:
	bool CanSeeOutSideFromCurrArea();
	bool CanSeeWaterFromCurrArea();
	void DrasticTidyUpMemory(bool);
	static void FinalShutdown();
	void GenerateTempPedAtStartOfNetworkGame();
	void Init1(char const*);
    void Init2(char const*);
	void Init3(char const*);
	void Initialise(char const *szFileName);
	void InitialiseCoreDataAfterRW();
	void InitialiseEssentialsAfterRW();
	static void InitialiseOnceBeforeRW();
	static bool InitialiseRenderWare();
	void InitialiseWhenRestarting();
	void Process();
	void ProcessTidyUpMemory();
	void ReInitGameObjectVariables();
	void ReloadIPLs();
	void ShutDownForRestart();
	void Shutdown();
	static void ShutdownRenderWare();
	void TidyUpMemory(bool, bool);
private:
	static char* aDatFile;
	static bool bMissionPackGame;
	static size_t currArea;
	static size_t currLevel;
	static RwMatrixTag *m_pWorkingMatrix1;
	static RwMatrixTag *m_pWorkingMatrix2;
};