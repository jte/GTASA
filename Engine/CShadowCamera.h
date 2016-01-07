#pragma once

class CShadowCamera
{
public:
	void ApplyAlphaMapToRaster();
    CShadowCamera();
	RwCamera* Create(int pow2);
	void Destroy();
    void DrawOutlineBorder(const RwRGBA& color);
    RwRaster* GetRwRenderRaster();
	RwTexture* GetRwRenderTexture();
    void InvertRaster();
	void MakeGradientRaster();
	void RasterBlur(RwRaster*);
	void RasterGradient(RwRaster* raster, int steps);
	void RasterResample(RwRaster* raster);
    void ReInitRasters();
	void SetCenter(RwV3d* center); 
    void SetFrustum(float frustum);
	void SetLight(RpLight* light);
	void ShadowRasterRender(RwV2d* viewWindow);
	/* Update(RpAtomic*)
	*/ 
    void Update(RpClump* clump);
	RwTexture* GetCameraFB();
	~CShadowCamera();
private:
    RwCamera* m_rwCamera;
    RwTexture* m_rwCameraFB;
};