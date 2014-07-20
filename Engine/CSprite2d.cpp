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
    RecipNearClip = 1.0 / Scene.Camera->nearPlane;
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
    if(IsVertexBufferFull())
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
