#pragma once

class CCustomBuildingRenderer
{
public:
	static void AtomicSetup(RpAtomic* atomic);
//	* CCustomBuildingRenderer()
	static bool Initialise();
	static bool IsCBPCPipelineAttached(RpAtomic* atomic);
	static bool PluginAttach();
	static void Shutdown();
	//* Update()
	static void UpdateDayNightBalanceParam();
	//* ~CCustomBuildingRenderer()
protected:
private:
};