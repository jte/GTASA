#pragma once

#include "CBaseModelInfo.h"
#include "CVisibilityPlugins.h"

/* struct name spelling error by GTA */
struct RwObjectNameIdAssocation
{
    char* name;
    int32_t hierarchyID;
    uint32_t flags;
};

class CClumpModelInfo : public CBaseModelInfo
{
public:
    // virtual methods
    virtual ~CClumpModelInfo();
    virtual size_t GetModelType();
    virtual void Init(); // DONE
    virtual void Shutdown(); // DONE
    virtual void DeleteRwObject(); // DONE
    virtual size_t GetRwModelType();
    virtual void* CreateInstance(RwMatrixTag* matrix); // DONE
    virtual void* CreateInstance(); // DONE
    virtual void SetAnimFile(char const* animFile); // DONE
    virtual void ConvertAnimFileIndex(); // DONE
	virtual size_t GetAnimFileIndex();
    virtual CColModel* GetBoundingBox(); // DONE
    virtual void* SetClump(RpClump* clump); // NEEDS WORK
    //
    static RwFrame* FillFrameArrayCB(RwFrame* frame, void* data);
    void FillFrameArray(RpClump* clump, RwFrame** array);
    static RwFrame* FindFrameFromNameCB(RwFrame* frame, void* data);
    static RwFrame* FindFrameFromNameWithoutIdCB(RwFrame* frame, void* data);
    static RwFrame* FindFrameFromIdCB(RwFrame* frame, void* data);
    static RpAtomic* SetAtomicRendererCB(RpAtomic* atomic, void* callback);
    RwFrame* GetFrameFromName(RpClump* clump, char const* name);
    RwFrame* GetFrameFromId(RpClump* clump, size_t id);
	void SetFrameIds(RwObjectNameIdAssocation* assoc);
private:
    union
    {
        int m_animId;
        char *m_animFile;
    };
};