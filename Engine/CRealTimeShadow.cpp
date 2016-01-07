#include "StdInc.h"

RwFrame* CRealTimeShadow::SetLightProperties(float angle, float, bool updateShadowCamera)
{
    const CVector xAxis(1.0f, 0.0f, 0.0f);
    const CVector yAxis(0.0f, 1.0f, 0.0f);
    RwFrame* frame = RpLightGetFrame(m_light);
    if (!frame)
    {
        return 0;
    }
    RwFrameRotate(frame, &yAxis, angle, rwCOMBINEREPLACE);
    RwFrameRotate(frame, &xAxis, -90.0, rwCOMBINEPOSTCONCAT);
    if (updateShadowCamera)
    {
        m_camera->SetLight(m_light);
    }
    return frame;
}

void CRealTimeShadow::Destroy()
{
	m_cameraNonBlurred.Destroy();
	m_cameraBlurred.Destroy();
	m_parent = NULL;
	m_objectType = -1;
	if (m_light)
	{
		RwFrame* frame = RwFrameGetParent(m_light);
		_rwObjectHasFrameSetFrame(frame, 0);
		RwFrameDestroy(frame);
		RpLightDestroy(m_light);
		m_light = 0;
	}
}

CShadowCamera* CRealTimeShadow::GetShadowCamera()
{
	return &m_cameraNonBlurred;
}

RwTexture* CRealTimeShadow::GetShadowRwTexture()
{
	if (m_isBlurred)
	{
		return m_cameraBlurred.GetCameraFB();
	}
	else
	{
		return m_cameraNonBlurred.GetCameraFB();
	}
}

void CRealTimeShadow::DrawBorderAroundTexture(const RwRGBA& color)
{
	if (m_isBlurred)
	{
		m_cameraBlurred.DrawOutlineBorder(color);
	}
	else
	{
		m_cameraNonBlurred.DrawOutlineBorder(color);
	}
}

bool CRealTimeShadow::Create(bool isBlurred, int gradientSteps, bool resampleRaster)
{
	m_light = RpLightCreate(1);
	if (!m_light)
	{
		return false;
	}
	RwRGBAReal color;
	color.red = 0.8f;
	color.green = 0.8f;
	color.blue = 0.8f;
	color.alpha = 0.0f;
	RpLightSetColor(m_light, &color);
	RwFrame* frame = RwFrameCreate();
	_rwObjectHasFrameSetFrame(m_light, frame);
	m_isBlurred = isBlurred;
	m_gradientSteps = gradientSteps;
	m_resampleRaster = resampleRaster;
	if (!isBlurred)
	{
		if (!m_cameraNonBlurred.Create(7))
		{
			Destroy();
			return false;
		}
	}
	else
	{
		if (!m_cameraBlurred.Create(6))
		{
			Destroy();
			return false;
		}
	}
	m_cameraNonBlurred.SetLight(m_light);
	return true;
}

bool CRealTimeShadow::SetShadowedObject(CPhysical* shadowedObject)
{
	m_shadowedObject = shadowedObject;
	RwObject* rwObject = m_shadowedObject->GetRwObject();
	if (rwObject == NULL)
	{
		return false;
	}
	m_objectType = RwObjectGetType(rwObject);
	if (m_objectType == RW_TYPE_ATOMIC) 
	{
		RpAtomic* atomic = (RpAtomic*)rwObject;
		if (atomic->interpolator.flags & 2)
		{
			_rpAtomicResyncInterpolatedSphere(atomic);
		}
		m_boundingSphere = atomic->boundingSphere;
		m_radius = m_boundingSphere.radius;
		RwV3dTransformPoints(&m_sunPosNormalized, &m_boundingSphere.center, 1, &RwFrameGetParent(atomic)->modelling);
	}
	else if (m_objectType == RW_TYPE_CLUMP)
	{
		RpClump* clump = (RpClump*)rwObject;
		RpClumpGetBoundingSphere(clump, &m_boundingSphere, 1);
		m_radius = m_boundingSphere.radius;
		RwV3dTransformPoints(&m_sunPosNormalized, &m_boundingSphere.center, 1, &RwFrameGetParent(clump)->modelling);
	}
	else 
	{
		Destroy();
		return false;
	}
	m_cameraNonBlurred.SetFrustum(m_boundingSphere.radius * 1.1f);
	m_cameraNonBlurred.SetCenter(&m_sunPosNormalized);
	return true;
}

RwTexture* CRealTimeShadow::Update()
{
	if (m_objectType == RW_TYPE_ATOMIC)
	{
		v8 = &this->m_shadowedObject->__parent.m_pRwObject->parent->modelling;
	}
	else if (m_objectType == RW_TYPE_CLUMP)
	{
		v8 = &this->m_shadowedObject->__parent.m_pRwObject->parent->modelling;
	}
	RwV3dTransformPoints(&m_sunPosNormalized, &m_boundingSphere.center, 1, v8);
	m_cameraNonBlurred.SetCenter(&m_sunPosNormalized);
	if (m_objectType == RW_TYPE_ATOMIC)
	{
		m_cameraNonBlurred.Update(m_shadowedObject->GetRwObject());
	}
	else if (m_objectType == RW_TYPE_CLUMP)
	{
		m_cameraNonBlurred.Update(m_shadowedObject->GetRwObject());
	}
	RwRaster* raster = m_cameraNonBlurred.GetRwRenderRaster();
	if (m_isBlurred)
	{
		m_cameraBlurred.RasterBlur(raster);
	}
	if (m_gradientSteps != 0)
	{
		CShadowCamera::RasterGradient(&g_realTimeShadowMan.pCamera1, v3, pThis->ucGradientSteps);
	}
	if (m_resampleRaster)
	{
		CShadowCamera::RasterResample(&g_realTimeShadowMan.pCamera2, v3);
	}
	return GetShadowRwTexture();
}

void CRealTimeShadow::Render(int a2, float a3)
{
	double v3; // st7@1
	double v5; // st6@1
	float v9; // [sp+Ch] [bp-8h]@1
	float v10; // [sp+10h] [bp-4h]@1

	v3 = (double)(a2 % 4) * 0.2;
	v5 = (double)(a2 / 4) * 0.2;
	v9 = v3 + 0.22;
	v10 = v5 + 0.22;
	RwV2d viewWindow;
	viewWindow.x = v3 + 0.02;
	viewWindow.y = v5 + 0.02;
	if (m_isBlurred)
	{
		m_cameraBlurred.ShadowRasterRender(&viewWindow);
	}
	else
	{
		m_cameraNonBlurred.ShadowRasterRender(&viewWindow);
	}
}

CRealTimeShadow::CRealTimeShadow()
{
	this->pCameraNonBlurred.m_rwCamera = 0;
	this->pCameraNonBlurred.m_rwCameraFB = 0;
	this->pCameraBlurred.m_rwCamera = 0;
	this->pCameraBlurred.m_rwCameraFB = 0;
	m_shadowedObject = NULL;
	m_objectType = -1;
	m_light = NULL;
	m_gradientSteps = 0;
	m_isBlurred = false;
	m_resampleRaster = false;
	m_gotRealTimeShadow = false;
	m_intensity = 0;
}

CPhysical* CRealTimeShadow::GetShadowedObject()
{
	return m_shadowedObject;
}

CRealTimeShadow::~CRealTimeShadow()
{
	Destroy();
	m_cameraBlurred.Destroy();
	m_cameraNonBlurred.Destroy();
}