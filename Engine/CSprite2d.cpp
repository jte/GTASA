#include "StdInc.h"

static RwImVertexIndex TempBufferRenderIndexList[16];
static RwD3D9Vertex TempVertexBuffer[1536];

void CSprite2d::SetRecipNearClip()
{
}

void CSprite2d::InitPerFrame()
{
    nextBufferVertex = 0;
    nextBufferIndex = 0;
    RecipNearClip = 1.0f / gScene.camera->nearPlane;
    NearScreenZ = RWSRCGLOBAL(dOpenDevice).zBufferNear;
}

bool CSprite2d::IsVertexBufferEmpty()
{
    return nextBufferVertex == 0;
}

bool CSprite2d::IsVertexBufferFull()
{
    return nextBufferVertex > 1532 || nextBufferIndex > 4090;
}

void CSprite2d::RenderVertexBuffer()
{
    if(nextBufferVertex > 0)
    {
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, TempVertexBuffer, nextBufferVertex, TempBufferRenderIndexList, nextBufferIndex);
        nextBufferVertex = 0;
        nextBufferIndex = 0;
    }
}

void CSprite2d::SetVertices(const CRect& rect, const CRGBA& v1, const CRGBA& v2, const CRGBA& v3, const CRGBA& v4)
{
    //  Screen drawing order:
    //  [3]             [2]
    //                  
    //                  
    //  [0]             [1]
    //  ===================
    //  Texture coords order:
    //  [2]             [3]
    //
    //  [1]             [0]
    //
    maVertices[0].x = rect.left;
    maVertices[0].y = rect.bottom;
    maVertices[0].z = NearScreenZ;
    maVertices[0].rhw = RecipNearClip;
    maVertices[0].u = 1.0f/1024.0f;
    maVertices[0].v = 1.0f/1024.0f;
    maVertices[0].emissiveColor = D3DCOLOR_RGBA(v3.red, v3.green, v3.blue, v3.alpha);
    maVertices[1].x = rect.right;
    maVertices[1].y = rect.bottom;
    maVertices[1].z = NearScreenZ;
    maVertices[1].rhw = RecipNearClip;
    maVertices[1].u = 1.0f/1000.0f;
    maVertices[1].v = 1.0f/1024.0f;
    maVertices[1].emissiveColor = D3DCOLOR_RGBA(v4.red, v4.green, v4.blue, v4.alpha);
    maVertices[2].x = rect.right;
    maVertices[2].y = rect.top;
    maVertices[2].z = NearScreenZ;
    maVertices[2].rhw = RecipNearClip;
    maVertices[2].u = 1.0f/1000.0f;
    maVertices[2].v = 1.0f/1000.0f;
    maVertices[2].emissiveColor = D3DCOLOR_RGBA(v2.red, v2.green, v2.blue, v2.alpha);
    maVertices[3].x = rect.left;
    maVertices[3].y = rect.top;
    maVertices[3].z = NearScreenZ;
    maVertices[3].rhw = RecipNearClip;
    maVertices[3].u = 1.0f/1024.0f;
    maVertices[3].v = 1.0f/1000.0f;
    maVertices[3].emissiveColor = D3DCOLOR_RGBA(v1.red, v1.green, v1.blue, v1.alpha);
}

void CSprite2d::DrawRect(const CRect& rect, const CRGBA& color)
{
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    SetVertices(rect, color, color, color, color);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)(color.alpha != 255));
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 4);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

void CSprite2d::AddToBuffer(const CRect& rect, const CRGBA& color, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
{
    SetVertices(&TempVertexBuffer[nextBufferVertex],
                rect,
                color, color, color, color,
                u1, v1,
                u2, v2,
                u3, v3,
                u4, v4);
    TempBufferRenderIndexList[nextBufferIndex + 0] = nextBufferVertex + 0;
    TempBufferRenderIndexList[nextBufferIndex + 1] = nextBufferVertex + 1;
    TempBufferRenderIndexList[nextBufferIndex + 2] = nextBufferVertex + 2;
    TempBufferRenderIndexList[nextBufferIndex + 3] = nextBufferVertex + 3;
    TempBufferRenderIndexList[nextBufferIndex + 4] = nextBufferVertex + 0;
    TempBufferRenderIndexList[nextBufferIndex + 5] = nextBufferVertex + 2;
    nextBufferVertex += 4;
    nextBufferIndex += 6;
    if (IsVertexBufferFull())
    {
        RenderVertexBuffer();
    }
}

CSprite2d::CSprite2d()
{
    m_texture = NULL;
}

void CSprite2d::Delete()
{
    if(m_texture)
    {
        RwTextureDestroy(m_texture);
        m_texture = NULL;
    }
}

void CSprite2d::SetTexture(char* name)
{
    Delete();
    if(name)
    {
        m_texture = RwTextureRead(name, NULL);
    }
}

void CSprite2d::SetTexture(char* name, char* mask)
{
    if(name && mask)
    {
        m_texture = RwTextureRead(name, mask);
    }
}

void CSprite2d::SetAddressingUV(RwTextureAddressMode u, RwTextureAddressMode v)
{
    if(m_texture)
    {
        m_texture->filterAddressing ^= (m_texture->filterAddressing ^ (u << 8)) & 0xF00;
        m_texture->filterAddressing ^= (m_texture->filterAddressing ^ (v << 12))& 0xF000;
    }
}

void CSprite2d::SetAddressing(RwTextureAddressMode addressing)
{
    if(m_texture)
    {
        m_texture->filterAddressing = m_texture->filterAddressing & 0xFFFF00FF | ((addressing & 0xF | 16 * (addressing & 0xF)) << 8);
    }
}

void CSprite2d::SetVertices(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const CRGBA& v1, const CRGBA& v2, const CRGBA& v3, const CRGBA& v4)
{
    //     Input order
    //  [0]             [1]
    //
    //
    //  [2]             [3]
    //  ======mapping======
    //  Screen coords order:
    //  [3]             [2]
    //
    // 
    //  [0]             [1]
    // ;;
    //  Texture coords order:
    //  [0]             [1]
    //                  
    //                  
    //  [3]             [2]
    maVertices[0].x = x2;
    maVertices[0].y = y2;
    maVertices[0].z = NearScreenZ;
    maVertices[0].rhw = RecipNearClip;
    maVertices[0].u = 0.0f;
    maVertices[0].v = 0.0f;
    maVertices[0].emissiveColor = D3DCOLOR_RGBA(v3.red, v3.green, v3.blue, v3.alpha);
    maVertices[1].x = x3;
    maVertices[1].y = y3;
    maVertices[1].z = NearScreenZ;
    maVertices[1].rhw = RecipNearClip;
    maVertices[1].u = 1.0f;
    maVertices[1].v = 0.0f;
    maVertices[1].emissiveColor = D3DCOLOR_RGBA(v4.red, v4.green, v4.blue, v4.alpha);
    maVertices[2].x = x1;
    maVertices[2].y = y1;
    maVertices[2].z = NearScreenZ;
    maVertices[2].rhw = RecipNearClip;
    maVertices[2].u = 1.0f;
    maVertices[2].v = 1.0f;
    maVertices[2].emissiveColor = D3DCOLOR_RGBA(v2.red, v2.green, v2.blue, v2.alpha);
    maVertices[3].x = x0;
    maVertices[3].y = y0;
    maVertices[3].z = NearScreenZ;
    maVertices[3].rhw = RecipNearClip;
    maVertices[3].u = 0.0f;
    maVertices[3].v = 1.0f;
    maVertices[3].emissiveColor = D3DCOLOR_RGBA(v1.red, v1.green, v1.blue, v1.alpha);
}

void CSprite2d::SetVertices(const CRect& rect, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
{
    // Screen draw order:
    // [3]          [2]
    //
    //
    // [0]          [1]
    maVertices[0].x = rect.left;
    maVertices[0].y = rect.bottom;
    maVertices[0].z = NearScreenZ;
    maVertices[0].u = u1;
    maVertices[0].v = v1;
    maVertices[0].rhw = RecipNearClip;
    maVertices[0].emissiveColor = D3DCOLOR_RGBA(c3.red, c3.green, c3.blue, c3.alpha);
    maVertices[1].x = rect.right;
    maVertices[1].y = rect.bottom;
    maVertices[1].z = NearScreenZ;
    maVertices[1].u = u2;
    maVertices[1].v = v2;
    maVertices[1].rhw = RecipNearClip;
    maVertices[1].emissiveColor = D3DCOLOR_RGBA(c4.red, c4.green, c4.blue, c4.alpha);
    maVertices[2].x = rect.right;
    maVertices[2].y = rect.top;
    maVertices[2].z = NearScreenZ;
    maVertices[2].u = u4;
    maVertices[2].v = v4;
    maVertices[2].rhw = RecipNearClip;
    maVertices[2].emissiveColor = D3DCOLOR_RGBA(c2.red, c2.green, c2.blue, c2.alpha);
    maVertices[3].x = rect.left;
    maVertices[3].y = rect.top;
    maVertices[3].z = NearScreenZ;
    maVertices[3].u = u3;
    maVertices[3].v = v3;
    maVertices[3].rhw = RecipNearClip;
    maVertices[3].emissiveColor = D3DCOLOR_RGBA(c1.red, c1.green, c1.blue, c1.alpha);
}

void CSprite2d::SetVertices(int count, float* xy, float* uv, const CRGBA& color)
{
    for (size_t i = 0; i < count; i++)
    {
        maVertices[i].x = xy[i+1];
        maVertices[i].y = xy[i+2];
        maVertices[i].z = CSprite2d::NearScreenZ + 0.0001f;
        maVertices[i].u = uv[i];
        maVertices[i].v = uv[i+1];
        maVertices[i].emissiveColor = D3DCOLOR_RGBA(color.red, color.green, color.blue, color.alpha);
        maVertices[i+1].x = CSprite2d::RecipNearClip;
    }
}

void CSprite2d::SetVertices(RwD3D9Vertex* buffer, const CRect& rect, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
{
    buffer[0].x = rect.left;
    buffer[0].y = rect.bottom;
    buffer[0].z = CSprite2d::NearScreenZ;
    buffer[0].rhw = CSprite2d::RecipNearClip;
    buffer[0].u = u1;
    buffer[0].v = v1;
    buffer[0].emissiveColor = D3DCOLOR_RGBA(c3.red, c3.green, c3.blue, c3.alpha);

    buffer[1].x = rect.right;
    buffer[1].y = rect.bottom;
    buffer[1].z = CSprite2d::NearScreenZ;
    buffer[1].rhw = CSprite2d::RecipNearClip;
    buffer[1].u = u2;
    buffer[1].v = v2;
    buffer[1].emissiveColor = D3DCOLOR_RGBA(c4.red, c4.green, c4.blue, c4.alpha);
  
    buffer[2].x = rect.right;
    buffer[2].y = rect.top;
    buffer[2].z = CSprite2d::NearScreenZ;
    buffer[2].rhw = CSprite2d::RecipNearClip;
    buffer[2].u = u4;
    buffer[2].v = v4;
    buffer[2].emissiveColor = D3DCOLOR_RGBA(c2.red, c2.green, c2.blue, c2.alpha);
  
    buffer[3].x = rect.left;
    buffer[3].y = rect.top;
    buffer[3].z = CSprite2d::NearScreenZ;
    buffer[3].rhw = CSprite2d::RecipNearClip;
    buffer[3].u = u3;
    buffer[3].v = v3;
    buffer[3].emissiveColor = D3DCOLOR_RGBA(c1.red, c1.green, c1.blue, c1.alpha);
}

void CSprite2d::SetRenderState()
{
    if (m_texture->raster)
    {
       RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_texture->raster->parent);
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    }
}

void CSprite2d::DrawCircleAtNearClip(const CVector2D& vector, float a2, const CRGBA& color, int slices)
{
    maVertices[0].x = vector.x;
    maVertices[0].y = vector.y;
    maVertices[0].z = CSprite2d::NearScreenZ;
    maVertices[0].rhw = CSprite2d::RecipNearClip;
    maVertices[0].u = 0.5f;
    maVertices[0].v = 0.5f;
    maVertices[0].emissiveColor = D3DCOLOR_RGBA(color.red, color.green, color.blue, color.alpha);
  
    float degrees_per_slice = 360.0f / slices;
  
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)true);
    for (size_t i = 0; i < slices; i++)
    {
        size_t slice_angle = DEG_TO_RAD(i * degrees_per_slice) / SINE_STEP;

        maVertices[1].x = a2 * CMaths::GetSin((slice_angle + 64) % 256 + 1) + vector.x;
        maVertices[1].y = a2 * CMaths::GetSin((slice_angle % 256) + 1) + vector.y;
        maVertices[1].z = NearScreenZ;
        maVertices[1].rhw = RecipNearClip;
        maVertices[1].u = (CMaths::GetSin((slice_angle + 64) % 256 + 1) + 1.0) * 0.5;
        maVertices[1].v = (CMaths::GetSin(slice_angle % 256 + 1) + 1.0) * 0.5;
        maVertices[1].emissiveColor = D3DCOLOR_RGBA(color.red, color.green, color.blue, color.alpha);
        
        slice_angle += DEG_TO_RAD(degrees_per_slice) / SINE_STEP;
        maVertices[2].x = a2 * CMaths::GetSin((slice_angle + 64) % 256 + 1) + vector.x;
        maVertices[2].y = a2 * CMaths::GetSin(slice_angle % 256 + 1) + vector.y;
        maVertices[2].z = NearScreenZ;
        maVertices[2].rhw = RecipNearClip;
        maVertices[2].u = (CMaths::GetSin((slice_angle + 64) % 256 + 1) + 1.0) * 0.5;
        maVertices[2].v = (CMaths::GetSin(slice_angle % 256 + 1) + 1.0) * 0.5;
        maVertices[2].emissiveColor = D3DCOLOR_RGBA(color.red, color.green, color.blue, color.alpha);
        
        RwIm2DRenderTriangle(maVertices, 3, 2, 1, 0);
    }
}

void CSprite2d::OffsetTexCoordForBilinearFiltering(float u, float v)
{
    float offsetU = 1.0 / (2 * u);
    float offsetV = 1.0 / (2 * v);
    maVertices[0].u += offsetU;
    maVertices[0].v += offsetV;
    maVertices[1].u += offsetU;
    maVertices[1].v += offsetV;
    maVertices[2].u += offsetU;
    maVertices[2].v += offsetV;
    maVertices[3].u += offsetU;
    maVertices[3].v += offsetV;
}

void CSprite2d::DrawWithBilinearOffset(const CRect& rect, const CRGBA& color)
{
    SetVertices(rect, color, color, color, color);
    OffsetTexCoordForBilinearFiltering(m_texture->raster->parent->width, m_texture->raster->parent->height);
    if (m_texture->raster)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_texture->raster->parent);
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
    }
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 4);
}

void CSprite2d::SetMaskVertices(int count, float* vertices, float z)
{
    for (size_t i = 0; i < count; i++)
    {
        maVertices[i].x = vertices[i];
        maVertices[i].y = vertices[i + 1];
        maVertices[i].z = z;
        maVertices[i].rhw = CSprite2d::RecipNearClip;
        maVertices[i].emissiveColor = D3DCOLOR_RGBA(0,0,0,0);
    }
}

void CSprite2d::DrawTxRect(const CRect& rect, const CRGBA& color)
{
    SetVertices(rect, color, color, color, color);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (m_texture->raster) ? m_texture->raster->parent : NULL);
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 4);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
}