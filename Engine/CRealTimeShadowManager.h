#pragma

class CRealTimeShadowManager
{
public:
	    /* CRealTimeShadowManager()
        * CamDistComp(void const*, void const*)
        * DoShadowThisFrame(CPhysical*)
        * Exit()
        * GetRealTimeShadow(CPhysical*)
        * ReInit()
        * 
        * Update()
        * ~CRealTimeShadowManager()
		*/
	void Init();
	CRealTimeShadow* ReturnRealTimeShadow(CRealTimeShadow* shadow);
private:
	bool m_initialized;
	uint8_t pad1[3];
	CRealTimeShadow* m_realTimeShadowPool[16];
	CShadowCamera m_camera1;
	CShadowCamera m_camera2;
};