#pragma once

class CClumpModelInfo : protected CBaseModelInfo
{
public:
	void ConvertAnimFileIndex();
	RpClump *CreateInstance();
	RpClump *CreateInstance(RwMatrixTag *pMatrix);
	void DeleteRwObject();
	//* FillFrameArray(RpClump*, RwFrame**)
	//* FillFrameArrayCB(RwFrame*, void*)
	//* FindFrameFromIdCB(RwFrame*, void*)
	//* FindFrameFromNameCB(RwFrame*, void*)
	//* FindFrameFromNameWithoutIdCB(RwFrame*, void*)
	//* GetAnimFileIndex()
	//* GetBoundingBox()
	//* GetFrameFromId(RpClump*, int)
	//* GetFrameFromName(RpClump*, char const*)
	//* GetModelType()
	//* GetRwModelType()
	void Init();
	void SetAnimFile(char const *szAnimFile);
	//* SetAtomicRendererCB(RpAtomic*, void*)
	//* SetClump(RpClump*)
	//* SetFrameIds(RwObjectNameIdAssocation*)
	void Shutdown();
	virtual ~CClumpModelInfo();
private:
    union
    {
        int m_AnimId;
        char *m_AnimFile;
    };
};