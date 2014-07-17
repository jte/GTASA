#pragma once

class CCustomBuildingRenderer
{
public:
	static void AtomicSetup(RpAtomic* atomic);
//	* CCustomBuildingRenderer()
	static bool Initialise();
	//* IsCBPCPipelineAttached(RpAtomic*)
	static bool PluginAttach();
	static void Shutdown();
	//* Update()
	//* UpdateDayNightBalanceParam()
	//* ~CCustomBuildingRenderer()
protected:
private:
};