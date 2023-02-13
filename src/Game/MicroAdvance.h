#pragma once

//=============================================================================
// Header
//=============================================================================
#include "MicroMath.h"

class ShaderProgram;
struct Texture2DInfo;
class Texture2D;
class Model;

//=============================================================================
// File Resource Cache System
//=============================================================================
namespace ResourceCacheSystem
{
	ShaderProgram* LoadShaderProgram(const char* fileName);
	Texture2D* LoadTexture2D(const char* fileName, const Texture2DInfo& textureInfo);

	Model* LoadModel(const char* fileName);
	
	bool IsLoad(const ShaderProgram& shader);
	bool IsLoad(const Texture2D& texture);

	void Clear();
} // ResourceCacheSystem

//=============================================================================
// Debug Draw
//=============================================================================

// ------------------------------------------------------------------------
// debugdraw framework
// Credits: Based on work by @glampert https://github.com/glampert/debug-draw (PD)
namespace DebugDraw
{
#define RGBX(rgb,x)   ( ((rgb)&0xFFFFFF) | (((unsigned)(x))<<24) )
#define RGB3(r,g,b)   ( ((r)<<16) | ((g)<<8) | (b) )
#define RGB4(r,g,b,a) RGBX(RGB3(r,g,b),a)

#define BLACK   RGBX(0x000000,255)
#define WHITE   RGBX(0xFFF1E8,255)
#define RED     RGB3(   255,48,48 )
#define GREEN   RGB3(  144,255,48 )
#define BLUE    RGBX(0x065AB5,255)
#define CYAN    RGB3(   0,192,255 )
#define ORANGE  RGB3(  255,144,48 )
#define PURPLE  RGB3( 178,128,255 )
#define YELLOW  RGB3(   255,224,0 )
#define GRAY    RGB3( 149,149,149 )
#define PINK    RGB3(  255,48,144 )
#define AQUA    RGB3(  48,255,144 )

	inline constexpr Vector3 RGBToVec(unsigned rgb)
	{
		return { ((rgb >> 16) & 255) / 255.f, ((rgb >> 8) & 255) / 255.f, ((rgb >> 0) & 255) / 255.f };
	}

	void DrawPoint(const Vector3& from, unsigned rgb);
	void DrawLine(const Vector3& from, const Vector3& to, unsigned rgb);
	void DrawLineDashed(Vector3 from, Vector3 to, unsigned rgb);

	void DrawAxis(float units);
	void DrawGround(float scale);
	void DrawGrid(float scale);

	void DrawTriangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, unsigned rgb);

	void DrawArrow(const Vector3& begin, const Vector3& end, unsigned rgb);
	void DrawBounds(const Vector3 points[8], unsigned rgb); // TODO: передавать points по ссылке, сейчас оно копируется
	void DrawBox(const Vector3& c, const Vector3& extents, unsigned rgb);
	void DrawCube(const Vector3& center, float radius, unsigned rgb);
	void DrawPlane(const Vector3& p, const Vector3& n, float scale, unsigned rgb);
	void DrawSphere(const Vector3& pos, float radius, unsigned rgb);
	void DrawCapsule(const Vector3& from, const Vector3& to, float radius, unsigned rgb);

	void DrawDiamond(const Vector3& from, const Vector3& to, float size, unsigned rgb);
	void DrawPyramid(const Vector3& center, float height, int segments, unsigned rgb);
	void DrawPrism(const Vector3& center, float radius, float height, const Vector3& normal, int segments, unsigned rgb);
	void DrawSquare(const Vector3& pos, float radius, unsigned rgb);
	void DrawCylinder(const Vector3& center, float height, int segments, unsigned rgb);
	void DrawPentagon(const Vector3& pos, float radius, unsigned rgb);
	void DrawHexagon(const Vector3& pos, float radius, unsigned rgb);
	void DrawCone(const Vector3& center, const Vector3& top, float radius, unsigned rgb);
	void DrawCircle(const Vector3& pos, const Vector3& n, float radius, unsigned rgb);
	void DrawAABB(const Vector3& minbb, const Vector3& maxbb, unsigned rgb);

	void DrawPosition(const Vector3& pos, float radius);
	void DrawPositionDir(const Vector3& pos, const Vector3& dir, float radius);
	void DrawNormal(const Vector3& pos, const Vector3& n);
	void DrawBone(const Vector3& center, const Vector3& end, unsigned rgb);
	void DrawBoid(const Vector3& pos, Vector3 dir);

	void Flush(const Matrix4& ViewProj);

	bool Init();
	void Close();
} // DebugDraw