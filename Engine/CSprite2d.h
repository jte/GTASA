#pragma once

class CSprite2d
{
public:
	void AddToBuffer(const CRect& rect, const CRGBA& color, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);
	CSprite2d();
	void Delete();
	//* Draw(CRect const&, CRGBA const&)
	//* Draw(CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* Draw(CRect const&, CRGBA const&, float, float, float, float, float, float, float, float)
	//* Draw(float, float, float, float, CRGBA const&)
	//* Draw(float, float, float, float, float, float, float, float, CRGBA const&)
	//* Draw2DPolygon(float, float, float, float, float, float, float, float, CRGBA const&)
	//* DrawAnyRect(float, float, float, float, float, float, float, float, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* DrawBarChart(float, float, unsigned short, unsigned char, float, signed char, unsigned char, unsigned char, CRGBA, CRGBA)
	//* DrawCircleAtNearClip(CVector2D const&, float, CRGBA const&, int)
	void DrawRect(const CRect& rect, const CRGBA& color);
	//* DrawRect(CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* DrawRectXLU(CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* DrawRotated(CRect&, float, CRGBA const&)
	//* DrawRotated(CVector2D const&, float, float, float, float, float, float, float, float, float, CRGBA const&)
	//* DrawTriangle(float*, float*, CRGBA const&)
	//* DrawTxRect(CRect const&, CRGBA const&)
	//* DrawWithBilinearOffset(CRect const&, CRGBA const&)
	static void InitPerFrame();
	static bool IsVertexBufferEmpty();
	static bool IsVertexBufferFull();
	//* OffsetTexCoordForBilinearFiltering(float, float)
	static void RenderVertexBuffer();
	void SetAddressing(RwTextureAddressMode addressing);
	void SetAddressingUV(RwTextureAddressMode u, RwTextureAddressMode v);
	//* SetMaskVertices(int, float*, float)
	static void SetRecipNearClip();
	//* SetRenderState()
	void SetTexture(char* name);
	void SetTexture(char* name, char* mask);
	static void SetVertices(const CRect& rect, const CRGBA& v1, const CRGBA& v2, const CRGBA& v3, const CRGBA& v4);
	static void SetVertices(const CRect& rect, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);
    //* SetVertices(RwD3D9Vertex*, CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&, float, float, float, float, float, float, float, float)
	static void SetVertices(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const CRGBA& v1, const CRGBA& v2, const CRGBA& v3, const CRGBA& v4);
	//* SetVertices(int, float*, CRGBA*)
	//* SetVertices(int count, float* xArray, float* yArray, const CRGBA& color)
	//* SetVerticesForSniper(CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* ~CSprite2d()
private:
	static float NearScreenZ;
	static float RecipNearClip;
	static RwD3D9Vertex maVertices[8];
	static size_t nextBufferIndex;
	static size_t nextBufferVertex;
    RwTexture* m_texture;
};