#pragma once

class CRealTimeShadow
{
public:
    CRealTimeShadow();
	void Render(int, float);
	bool Create(bool isBlurred, int gradientSteps, bool resampleRaster);
	void Destroy();
	void DrawBorderAroundTexture(const RwRGBA& color);
    RwFrame* SetLightProperties(float angle, float, bool updateShadowCamera);
	CShadowCamera* GetShadowCamera();
	RwTexture* GetShadowRwTexture();
	bool SetShadowedObject(CPhysical* shadowedObject);
	RwTexture* Update();
	CPhysical* GetShadowedObject();
	~CRealTimeShadow();
private:
	CPhysical* m_shadowedObject;
	bool m_gotRealTimeShadow;
	uint8_t m_intensity;
	uint8_t pad1;
	uint8_t pad2;
	CShadowCamera m_cameraNonBlurred;
	bool m_isBlurred;
	uint8_t pad3;
	uint8_t pad4;
	uint8_t pad5;
	CShadowCamera m_cameraBlurred;
	uint32_t m_gradientSteps;
	bool m_resampleRaster;
	uint8_t pad6;
	uint8_t pad7;
	uint8_t pad8;
	uint32_t m_objectType;
	RpLight* m_light;
	RwSphere m_boundingSphere;
	RwV3d m_sunPosNormalized;
	float m_radius;

    CShadowCamera* m_camera;
    RpLight* m_light;
};