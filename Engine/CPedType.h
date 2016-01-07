#pragma once

class CPedType
{
public:
	/* ClearPedTypeAcquaintances(int, int)
	* ClearPedTypeAsAcquaintance(int, int, unsigned int)
	*/ 
	static int FindPedType(char*);
	static uint32_t GetPedFlag(int n);
	static CAcquaintance* GetPedTypeAcquaintances(int pedType);
	/* GetPedTypeAcquaintances(int, int)
	*/ 
	static void Initialise();
	/* Load()
	*/ 
	static void LoadPedData();
	static bool PoliceDontCareAboutCrimesAgainstPedType(int pedType);
	/* Save()
	* SetPedTypeAsAcquaintance(int, int, unsigned int)
	*/ 
	static void Shutdown();
private:
	static CAcquaintance* ms_apPedTypes;
};