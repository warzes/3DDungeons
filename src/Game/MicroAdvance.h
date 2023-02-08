#pragma once

//=============================================================================
// Header
//=============================================================================
#include "MicroMath.h"
#include "MicroMathOld.h"

class ShaderProgram;
struct Texture2DInfo;
class Texture2D;
class Model;

//=============================================================================
// Config
//=============================================================================
#define USE_SIMPLE_PERLIN_NOISE 1

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

inline constexpr Vector3Old RGBToVec(unsigned rgb)
{
	return { ((rgb >> 16) & 255) / 255.f, ((rgb >> 8) & 255) / 255.f, ((rgb >> 0) & 255) / 255.f };
}

namespace DebugDraw
{
	// ------------------------------------------------------------------------
	// debugdraw framework
	// Credits: Based on work by @glampert https://github.com/glampert/debug-draw (PD)
	// [x] grid, axis, frustum, cube, sphere, triangle, square, pentagon, hexagon, circle, normal.
	// [x] arrow, point, text, capsule, aabb, plane, flotilla-style locator, boid,
	// [x] line batching
	// [*] line width and stipple
	// [*] (proper) gizmo,
	// [ ] bone (pyramid? two boids?), ring,
	// [ ] camera, light bulb, light probe,

	void DrawPoint(const Vector3Old& from, unsigned rgb);
	void DrawLine(const Vector3Old& from, const Vector3Old& to, unsigned rgb);
	void DrawLineDashed(Vector3Old from, Vector3Old to, unsigned rgb);

	void DrawAxis(float units);
	void DrawGround(float scale);
	void DrawGrid(float scale);

	void DrawTriangle(const Vector3Old& p1, const Vector3Old& p2, const Vector3Old& p3, unsigned rgb);

	void DrawArrow(const Vector3Old& begin, const Vector3Old& end, unsigned rgb);
	void DrawBounds(const Vector3Old points[8], unsigned rgb); // TODO: передавать points по ссылке, сейчас оно копируется
	void DrawBox(const Vector3Old& c, const Vector3Old& extents, unsigned rgb);
	void DrawCube(const Vector3Old& center, float radius, unsigned rgb);
	void DrawPlane(const Vector3Old& p, const Vector3Old& n, float scale, unsigned rgb);
	void DrawSphere(const Vector3Old& pos, float radius, unsigned rgb);
	void DrawCapsule(const Vector3Old& from, const Vector3Old& to, float radius, unsigned rgb);

	void DrawDiamond(const Vector3Old& from, const Vector3Old& to, float size, unsigned rgb);
	void DrawPyramid(const Vector3Old& center, float height, int segments, unsigned rgb);
	void DrawPrism(const Vector3Old& center, float radius, float height, const Vector3Old& normal, int segments, unsigned rgb);
	void DrawSquare(const Vector3Old& pos, float radius, unsigned rgb);
	void DrawCylinder(const Vector3Old& center, float height, int segments, unsigned rgb);
	void DrawPentagon(const Vector3Old& pos, float radius, unsigned rgb);
	void DrawHexagon(const Vector3Old& pos, float radius, unsigned rgb);
	void DrawCone(const Vector3Old& center, const Vector3Old& top, float radius, unsigned rgb);
	void DrawCircle(const Vector3Old& pos, const Vector3Old& n, float radius, unsigned rgb);
	void DrawAABB(const Vector3Old& minbb, const Vector3Old& maxbb, unsigned rgb);

	void DrawPosition(const Vector3Old& pos, float radius);
	void DrawPositionDir(const Vector3Old& pos, const Vector3Old& dir, float radius);
	void DrawNormal(const Vector3Old& pos, const Vector3Old& n);
	void DrawBone(const Vector3Old& center, const Vector3Old& end, unsigned rgb);
	void DrawBoid(const Vector3Old& pos, Vector3Old dir);

	void Flush(const Matrix4Old& ViewProj);

	bool Init();
	void Close();
} // DebugDraw

//=============================================================================
// Simple Perlin Noise
//=============================================================================
#if USE_SIMPLE_PERLIN_NOISE

#endif // USE_SIMPLE_PERLIN_NOISE