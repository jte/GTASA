#include "StdInc.h"

void CMenuManager::LoadSettings()
{
    CFileMgr::SetDirMyDocuments();
    v16 = CFileMgr::OpenFile("gta_sa.set", "rb");
    v17 = v16;
    if ( v17 )
    {
    if ( !CInputEvents__readFromSettingsFile(v17) )
        CControllerConfigManager::ReinitControls();
    CFileMgr::CloseFile(v17);
    }

    CFileMgr::SetDir("");
}

void CMenuManager::MessageScreen(char* message, char a2, char a3)
{
	if (!a3)
	{
		if (!RwCameraBeginUpdate(gScene.camera))
		{
			return;
		}
		if (a2)
		{
			CRect rect;
			rect.left = 0.0f;
			rect.top = RsGlobal.MaximumHeight;
			rect.right = RsGlobal.MaximumWidth;
			rect.bottom = 0.0f;
			CRGBA color(0, 0, 0, 255);
			CSprite2d::DrawRect(rect, color);
		}
	}
	CSprite2d::SetRecipNearClip();
	CSprite2d::InitPerFrame();
	CFont::InitPerFrame();
	DefinedState2d();
	if (a2)
	{
		CRect rect;
		rect.left = 0.0f;
		rect.top = RsGlobal.MaximumHeight;
		rect.right = RsGlobal.MaximumWidth;
		rect.bottom = 0.0f;
		CRGBA color(0, 0, 0, 255);
		CSprite2d::DrawRect(rect, color);
	}
	SmallMessageScreen(message);
	dummy_71A210();
}