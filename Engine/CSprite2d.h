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
	void DrawCircleAtNearClip(const CVector2D& vector, float a2, const CRGBA& color, int slices);
	static void DrawRect(const CRect& rect, const CRGBA& color);
	//* DrawRect(CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* DrawRectXLU(CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* DrawRotated(CRect&, float, CRGBA const&)
	//* DrawRotated(CVector2D const&, float, float, float, float, float, float, float, float, float, CRGBA const&)
	//* DrawTriangle(float*, float*, CRGBA const&)
	void DrawTxRect(const CRect& rect, const CRGBA& color);
	void DrawWithBilinearOffset(const CRect& rect, const CRGBA& color);
	static void InitPerFrame();
	static bool IsVertexBufferEmpty();
	static bool IsVertexBufferFull();
	static void OffsetTexCoordForBilinearFiltering(float u, float v);
	static void RenderVertexBuffer();
	void SetAddressing(RwTextureAddressMode addressing);
	void SetAddressingUV(RwTextureAddressMode u, RwTextureAddressMode v);
	static void SetMaskVertices(int count, float* vertices, float z);
	static void SetRecipNearClip();
	void SetRenderState();
	void SetTexture(char* name);
	void SetTexture(char* name, char* mask);
	static void SetVertices(const CRect& rect, const CRGBA& v1, const CRGBA& v2, const CRGBA& v3, const CRGBA& v4);
	static void SetVertices(const CRect& rect, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);
    static void SetVertices(RwD3D9Vertex* buffer, const CRect& rect, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);
	static void SetVertices(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const CRGBA& v1, const CRGBA& v2, const CRGBA& v3, const CRGBA& v4);
	//* SetVertices(int, float*, CRGBA*)
	static void SetVertices(int count, float* xy, float* uv, const CRGBA& color);
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