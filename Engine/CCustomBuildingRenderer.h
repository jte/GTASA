#pragma once

class CCustomBuildingRenderer
{
public:
	void AtomicSetup(RpAtomic* atomic);
//	* CCustomBuildingRenderer()
	bool Initialise();
	//* IsCBPCPipelineAttached(RpAtomic*)
	bool PluginAttach();
	void Shutdown();
	//* Update()
	//* UpdateDayNightBalanceParam()
	//* ~CCustomBuildingRenderer()
protected:
private:
};