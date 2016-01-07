#include "StdInc.h"

CShadowCamera::CShadowCamera()
{
    m_rwCamera = NULL;
    m_rwCameraFB = NULL;
}

void CShadowCamera::ReInitRasters()
{
    int32_t dim = m_rwCamera->frameBuffer->width;
    RwRasterDestroy(m_rwCamera->frameBuffer);
    m_rwCamera->frameBuffer = NULL;
    m_rwCamera->frameBuffer = RwRasterCreate(dim, dim, 0, rwRASTERTYPECAMERATEXTURE); 
    RwTextureSetRaster(m_rwCameraFB, m_rwCamera->frameBuffer);
}

RwTexture* CShadowCamera::GetCameraFB()
{
	return m_rwCameraFB;
}

void CShadowCamera::SetFrustum(float frustum)
{
    RwCameraSetFarClipPlane(m_rwCamera, 2 * frustum);
    RwCameraSetNearClipPlane(m_rwCamera, frustum / 1000);
    RwV2d viewWindow;
    viewWindow.x = frustum;
    viewWindow.y = frustum;
    RwCameraSetViewWindow(m_rwCamera, &viewWindow);
}

void CShadowCamera::SetLight(RpLight* light)
{
    RwFrame* lightFrame = RpLightGetFrame(light);
    RwFrame* cameraFrame = RwCameraGetFrame(m_rwCamera);
    cameraFrame->modelling.right = lightFrame->modelling.right;
    cameraFrame->modelling.up = lightFrame->modelling.up;
    cameraFrame->modelling.at = lightFrame->modelling.at;
    RwMatrixUpdate(&cameraFrame->modelling);
    RwFrameUpdateObjects(cameraFrame);
}

void CShadowCamera::SetCenter(RwV3d* center)
{
    RwFrame* cameraFrame = RwCameraGetFrame(m_rwCamera);
    CVector pos = *center;
    pos += CVector(cameraFrame->modelling.at) * -0.5 * m_rwCamera->farPlane;
    cameraFrame->modelling.pos = pos;
    RwMatrixUpdate(&cameraFrame->modelling);
    RwFrameUpdateObjects(cameraFrame);
    RwFrameOrthoNormalize(cameraFrame);
}

void CShadowCamera::InvertRaster()
{
    RwIm2DVertex vertices[4];

    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[0].z = RwIm2DGetNearScreenZ();
    vertices[0].emissiveColor = D3DCOLOR_RGBA(255, 255, 255, 255);
    vertices[0].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);

    vertices[1].x = 0;
    vertices[1].y = m_rwCamera->frameBuffer->height;
    vertices[1].z = RwIm2DGetNearScreenZ();
    vertices[1].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[1].emissiveColor = D3DCOLOR_RGBA(255, 255, 255, 255);

    vertices[2].y = 0;
    vertices[2].x = m_rwCamera->frameBuffer->width;
    vertices[2].z = RwIm2DGetNearScreenZ();
    vertices[2].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[2].emissiveColor = D3DCOLOR_RGBA(255, 255, 255, 255);

    vertices[3].x = m_rwCamera->frameBuffer->width;
    vertices[3].y = m_rwCamera->frameBuffer->height;
    vertices[3].z = RwIm2DGetNearScreenZ();
    vertices[3].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[3].emissiveColor = D3DCOLOR_RGBA(255, 255, 255, 255);

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVDESTCOLOR);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vertices, 4);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
}

RwRaster* CShadowCamera::GetRwRenderRaster()
{
    return m_rwCamera->frameBuffer;
}

RwTexture* CShadowCamera::GetRwRenderTexture()
{
    return m_rwCameraFB;
}

void CShadowCamera::DrawOutlineBorder(const RwRGBA& color)
{
    RwImVertexIndex indices[5];
    RwIm2DVertex vertices[4];

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;
    indices[4] = 0;

    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[1].y = 0;
    vertices[3].x = 0;

    vertices[0].emissiveColor = D3DCOLOR_RGBA(color.red, color.green, color.blue, color.alpha);
    vertices[1].emissiveColor = vertices[0].emissiveColor;
    vertices[2].emissiveColor = vertices[0].emissiveColor;
    vertices[3].emissiveColor = vertices[0].emissiveColor;

    vertices[0].z = RwIm2DGetNearScreenZ();
    vertices[3].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[0].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[1].x = m_rwCamera->frameBuffer->width - 1.0;
    vertices[1].z = RwIm2DGetNearScreenZ();
    vertices[1].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[2].x = m_rwCamera->frameBuffer->width - 1.0;
    vertices[2].y = m_rwCamera->frameBuffer->width - 1.0;
    vertices[2].z = RwIm2DGetNearScreenZ();
    vertices[2].rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);
    vertices[3].y = m_rwCamera->frameBuffer->width - 1.0;
    vertices[3].z = RwIm2DGetNearScreenZ();
    if (RwCameraBeginUpdate(m_rwCamera))
    {
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPEPOLYLINE, vertices, 4, indices, 5);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwCameraEndUpdate(m_rwCamera);
    }
}

RwCamera* CShadowCamera::Create(int pow2)
{
    m_rwCamera = RwCameraCreate();
    if (!m_rwCamera)
    {
        Destroy();
        return NULL;
    }
    RwFrame* frame = RwFrameCreate();
    RwCameraSetFrame(m_rwCamera, frame);
    if (!RwCameraGetFrame(m_rwCamera))
    {
        Destroy();
        return NULL;
    }
    RwRaster* raster = RwRasterCreate(1 << pow2, 1 << pow2, 0, 5);
    if (!raster)
    {
        Destroy();
        return NULL;
    }
    m_rwCamera->frameBuffer = raster;
    m_rwCameraFB = RwTextureCreate(m_rwCamera->frameBuffer);
    if (!m_rwCameraFB)
    {
        Destroy();
        return NULL;
    }
    RwTextureSetAddressing(m_rwCameraFB, rwTEXTUREADDRESSCLAMP);
    RwTextureSetFilterMode(m_rwCameraFB, rwFILTERLINEAR);
    RwCameraSetProjection(m_rwCamera, rwPARALLEL);
    return m_rwCamera;
}

void CShadowCamera::Destroy()
{
    if (!m_rwCamera)
    {
        return;
    }
    RwFrame* cameraFrame = RwCameraGetFrame(m_rwCamera);
    if (cameraFrame)
    {
        RwFrameDestroy(cameraFrame);
        RwCameraSetFrame(m_rwCamera, NULL);
    }
    if (m_rwCamera->frameBuffer)
    {
        RwRasterDestroy(m_rwCamera->frameBuffer);
        m_rwCamera->frameBuffer = NULL;
    }
    if (m_rwCameraFB)
    {
        RwTextureSetRaster(m_rwCameraFB, NULL);
        RwTextureDestroy(m_rwCameraFB);
        m_rwCameraFB = NULL;
    }
    RwCameraDestroy(m_rwCamera);
    m_rwCamera = 0;
}

void CShadowCamera::ApplyAlphaMapToRaster()
{
    if (!m_rwCamera->frameBuffer)
    {
        return;
    }
    RwImage* image = RwImageCreate(m_rwCamera->frameBuffer->width, m_rwCamera->frameBuffer->height, 32);
    if (!image)
    {
        return;
    }
    RwImageAllocatePixels(image);
    InvertRaster();
    RwImageSetFromRaster(image, m_rwCamera->frameBuffer);
    RwImageMakeMask(image);
    RwRasterSetFromImage(m_rwCamera->frameBuffer, image);
    RwImageDestroy(image);
}

static RpAtomic* ShadowCameraAtomicRender(RpAtomic* atomic,void*)
{
    RpAtomicCallBackRender savedCB = atomic->renderCallBack;
    atomic->renderCallBack = AtomicDefaultRenderCallBack;
    if (!AtomicDefaultRenderCallBack)
    {
        atomic->renderCallBack = AtomicDefaultRenderCallBack;
    }
    AtomicDefaultRenderCallBack(atomic);
    atomic->renderCallBack = savedCB;
    if (!savedCB)
    {
        atomic->renderCallBack = AtomicDefaultRenderCallBack;
    }
    return atomic;
}

void CShadowCamera::Update(RpClump* clump)
{
    RwRGBA colour;
    colour.red = 255;
    colour.green = 255;
    colour.blue = 255;
    colour.alpha = 0;
    RwCameraClear(m_rwCamera, &colour, rwCAMERACLEARIMAGE | rwCAMERACLEARZ); 
    if ( RwCameraBeginUpdate(m_rwCamera) )
    {
        RpGeometry* geometry = GetFirstAtomic(clump)->geometry;
        uint32_t savedFlags = geometry->flags;
        geometry->flags &= rpGEOMETRYNORMALS | rpGEOMETRYTRISTRIP | rpGEOMETRYPOSITIONS; 
        RpClumpForAllAtomics(clump, ShadowCameraAtomicRender, 0);
        geometry->flags = savedFlags;
        InvertRaster();
        RwCameraEndUpdate(m_rwCamera);
    }
}

void Im2DRenderQuad(int left, int top, int right, int bottom, int z, int rhw, float uv)
{
    RwD3D9Vertex vertices[4];
    
    vertices[0].y = top;
    vertices[1].v = uv + 1.0;
    vertices[0].z = z;
    vertices[2].u = uv + 1.0;
    vertices[3].u = uv + 1.0;
    vertices[0].rhw = rhw;
    vertices[3].v = v7;
    vertices[0].v = uv + 1.0;
    vertices[0].u = uv;
    vertices[1].y = bottom;
    vertices[1].x = left;
    vertices[1].rhw = rhw;
    vertices[1].z = z;
    vertices[2].x = right;
    vertices[1].u = uv;
    vertices[2].z = z;
    vertices[2].y = top;
    vertices[2].v = uv;
    vertices[0].x = left;
    vertices[2].rhw = rhw;
    vertices[3].y = bottom;
    vertices[0].emissiveColor = -1;
    vertices[1].emissiveColor = -1;
    vertices[2].emissiveColor = -1;
    vertices[3].x = right;
    vertices[3].emissiveColor = -1;
    vertices[3].z = z;
    vertices[3].rhw = rhw;
    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vertices, ELEMS_COUNT(vertices));
}

void CShadowCamera::ShadowRasterRender(RwV2d* viewWindow)
{
    float x1 = 0.83f;
    float y1 = 0.02f;
    float x2 = 0.98f;
    float y2 = 0.17f;
    if (viewWindow)
    {
        x1 = viewWindow[0].x;
        y1 = viewWindow[0].y;
        x2 = viewWindow[1].x;
        y2 = viewWindow[1].y;
    }
    RwCamera* curCamera = (RwCamera*)RWSRCGLOBAL(curCamera);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_rwCamera->frameBuffer);
    float bottom = curCamera->frameBuffer->height * y2;
    float right = curCamera->frameBuffer->width * x2;
    float top = curCamera->frameBuffer->height * y1;
    float left = curCamera->frameBuffer->width * x1;
    Im2DRenderQuad(left, top, right, bottom, RwIm2DGetNearScreenZ(), 1.0 / curCamera->nearPlane, 0.0);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
}

void CShadowCamera::MakeGradientRaster()
{
    if (!m_rwCamera)
    {
        return;
    }
    if (m_rwCamera->frameBuffer->height == 0)
    {
        return;
    }
    if (!RwCameraBeginUpdate(m_rwCamera))
    {
        return;
    }
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVDESTCOLOR);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
    static size_t gradientStart = 0x80;
    static size_t gradientEnd = 0x40;
    int gradientStep = (gradientEnd - gradientStart) / m_rwCamera->frameBuffer->height;
    int col = gradientStart;
    
    for (size_t i = 0; i < m_rwCamera->frameBuffer->height; i++)
    {
        RwD3D9Vertex vertices[2];
        vertices[0].x = 0;
        vertices[0].y = i;
        vertices[0].z = RwIm2DGetNearScreenZ();
        vertices[0].rhw = 1.0 / m_rwCamera->nearPlane;
        vertices[0].emissiveColor = D3DCOLOR_RGBA(col,col,col,col);
        vertices[1].y = i;
        vertices[1].emissiveColor = vertices[0].emissiveColor;
        vertices[1].x = m_rwCamera->frameBuffer->width - 1;
        vertices[1].z = RwIm2DGetNearScreenZ();
        vertices[1].rhw = 1.0 / m_rwCamera->nearPlane;
        RwIm2DRenderLine(vertices, 2, 0, 1);
        col += gradientStep;
    }
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
    RwCameraEndUpdate(m_rwCamera);
}

void CShadowCamera::RasterBlur(RwRaster* raster)
{
    if (!m_rwCamera)
    {
        return;
    }
    if (!RwCameraBeginUpdate(m_rwCamera))
    {
        return;
    }
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
    int width = m_rwCamera->frameBuffer->width;
    Im2DRenderQuad(0, 0, width, width, RwIm2DGetNearScreenZ(), 1.0 / RwCameraGetNearClipPlane(m_rwCamera), 1 / (2*width));
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwCameraEndUpdate(m_rwCamera);
}

void CShadowCamera::RasterGradient(RwRaster* raster, int steps)
{
    if (!m_rwCamera)
    {
        return;
    }

    RwRaster* prevRaster = m_rwCamera->frameBuffer;
    float rhw = 1.0 / RwCameraGetNearClipPlane(m_rwCamera);

    for (size_t i = 0; i < steps; i++)
    {
        m_rwCamera->frameBuffer = prevRaster;
        if (RwCameraBeginUpdate(m_rwCamera))
        {
            if (i == 0)
            {
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
                RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
                RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
            }
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
            Im2DRenderQuad(0, 0, raster->width, raster->width, RwIm2DGetNearScreenZ(), rhw, (0.5 + 0.5) / raster->width);
            RwCameraEndUpdate(m_rwCamera);
        }
        m_rwCamera->frameBuffer = raster;
        if (!RwCameraBeginUpdate(m_rwCamera))
        {
            continue;
        }
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, prevRaster);
        Im2DRenderQuad(0, 0, raster->width, raster->width, RwIm2DGetNearScreenZ(), rhw, (0.5 - 0.5) / raster->width); /* GTA bug?*/
        if (i == steps - 1)
        {
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        }
        RwCameraEndUpdate(m_rwCamera);
    }
    m_rwCamera->frameBuffer = prevRaster;
}

void CShadowCamera::RasterResample(RwRaster* raster)
{
    RwRaster* prevRaster = m_rwCamera->frameBuffer;
    m_rwCamera->frameBuffer = raster;
    if (!RwCameraBeginUpdate(m_rwCamera))
    {
        return;
    }
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDSRCCOLOR);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_rwCamera->frameBuffer);
    Im2DRenderQuad(0, 0, raster->width, raster->width, RwIm2DGetNearScreenZ(), 1.0 / RwCameraGetNearClipPlane(m_rwCamera), 0.0);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwCameraEndUpdate(m_rwCamera);
    m_rwCamera->frameBuffer = prevRaster;
}

CShadowCamera::~CShadowCamera()
{
    Destroy();
}