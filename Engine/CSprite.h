#pragma once

class CSprite
{
public:
	static float CalcHorizonCoors();
	static bool CalcScreenCoors(const RwV3d& worldPos, RwV3d* viewPos, float* screenX, float* screenY, bool checkFarClip, bool checkNearClip);
	//* Draw3DSprite(float, float, float, float, float, float, float, float, float)
	//* FlushSpriteBuffer()
	static void InitSpriteBuffer(); 
	static void InitSpriteBuffer2D();
	//* Initialise()
	//* RenderBufferedOneXLUSprite(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, unsigned char)
	//* RenderBufferedOneXLUSprite2D(float, float, float, float, RwRGBA const&, short, unsigned char)
	//* RenderBufferedOneXLUSprite2D_Rotate_Dimension(float, float, float, float, RwRGBA const&, short, float, unsigned char)
	//* RenderBufferedOneXLUSprite_Rotate_2Colours(float, float, float, float, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, float, float, float, float, unsigned char)
	//* RenderBufferedOneXLUSprite_Rotate_Aspect(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, float, unsigned char)
	//* RenderBufferedOneXLUSprite_Rotate_Dimension(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, float, unsigned char)
	//* RenderOneXLUSprite(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, unsigned char, unsigned char, unsigned char, float, float)
	//* RenderOneXLUSprite2D(float, float, float, float, RwRGBA const&, short, unsigned char)
	//* RenderOneXLUSprite2D_Rotate_Dimension(float, float, float, float, RwRGBA const&, short, float, unsigned char)
	//* RenderOneXLUSprite_Rotate_2Colours(float, float, float, float, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, float, float, float, float, unsigned char)
	static void RenderOneXLUSprite_Rotate_Aspect(float viewPosX, float viewPosY, float viewPosZ, float width, float height, uint8_t red, uint8_t green, uint8_t blue, short, float, float, uint8_t);
	//* RenderOneXLUSprite_Rotate_Dimension(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, float, unsigned char)
	//* RenderOneXLUSprite_Triangle(float, float, float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, unsigned char)
	//* Set4Vertices2D(RwOpenGLVertex*, CRect const&, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
	//* Set4Vertices2D(RwOpenGLVertex*, float, float, float, float, float, float, float, float, CRGBA const&, CRGBA const&, CRGBA const&, CRGBA const&)
private:
	static bool m_bFlushSpriteBufferSwitchZTest;
	static float m_f2DFarScreenZ;
	static float m_f2DNearScreenZ;
	static float m_fRecipNearClipPlane;
};