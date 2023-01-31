#pragma once

//=============================================================================
// Header
//=============================================================================
#include <math.h>

//=============================================================================
// Core
//=============================================================================
constexpr float EPSILON = 1e-6f;
constexpr float PI = 3.14159265358979323846f;
constexpr float HALF_PI = PI * 0.5f;
constexpr float DEG2RAD = PI / 180.0f;
constexpr float RAD2DEG = 180.0f / PI;

inline constexpr int   Min(int a, int b) { return a < b ? a : b; }
inline constexpr float Min(float a, float b) { return a < b ? a : b; }
inline constexpr int   Max(int a, int b) { return a > b ? a : b; }
inline constexpr float Max(float a, float b) { return a > b ? a : b; }
inline constexpr float Clamp(float x, float min, float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline bool Ñompare(float f1, float f2) { return fabsf(f1 - f2) < EPSILON; }

//=============================================================================
// Point2
//=============================================================================
class Point2
{
public:
	int x = 0;
	int y = 0;
};

//=============================================================================
// Color
//=============================================================================
class Color
{
public:
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
};

//=============================================================================
// Vector2
//=============================================================================
class Vector2
{
public:
	float x = 0.0f;
	float y = 0.0f;
};

//=============================================================================
// Vector3
//=============================================================================
class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

//=============================================================================
// Matrix4
//=============================================================================

class Matrix4
{
public:
	constexpr Matrix4() = default;
	constexpr Matrix4(const Matrix4&) = default;

	constexpr Matrix4& operator=(const Matrix4&) = default;

	constexpr float& operator[](size_t i) { return m[i]; }
	constexpr const float operator[](size_t i) const { return m[i]; }

	[[nodiscard]] static Matrix4 GetPerspective(float fieldOfView, float aspectRatio, float znear, float zfar);

	float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
};

inline Matrix4 Matrix4::GetPerspective(float fieldOfView, float aspectRatio, float znear, float zfar)
{
	const float y = tanf(fieldOfView / 2.0f);
	const float x = y * aspectRatio;
	Matrix4 mat;
	mat[0] = 1.0f / x; mat[4] = 0.0f;     mat[8] = 0.0f;                              mat[12] = 0.0f;
	mat[1] = 0.0f;     mat[5] = 1.0f / y; mat[9] = 0.0f;                              mat[13] = 0.0f;
	mat[2] = 0.0f;     mat[6] = 0.0f;     mat[10] = -(zfar + znear) / (zfar - znear); mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
	mat[3] = 0.0f;     mat[7] = 0.0f;     mat[11] = -1.0f;                            mat[15] = 0.0f;
	return mat;
}