#pragma once

exengine\math
FWK\code\split
Engine 0.2
https://github.com/ProfJski/RaylibOpOverloads
https://github.com/felselva/mathc

/*
Left handed
  1 Y   Z
    |  / 1
    | /
    |/___X
   0     1
*/

//=============================================================================
// Header
//=============================================================================
#include <math.h>

//=============================================================================
// Constant definitions
//=============================================================================
constexpr const float    EPSILON = 1.0e-6f;
constexpr const int      MIN_INT = 0x80000000;
constexpr const int      MAX_INT = 0x7fffffff;
constexpr const unsigned MIN_UNSIGNED = 0x00000000;
constexpr const unsigned MAX_UNSIGNED = 0xffffffff;
constexpr const float    MAX_FLOAT = 3.402823466e+38f;
constexpr const float    PI = 3.14159265358979323846f;
constexpr const float    HALF_PI = PI * 0.5f;
constexpr const float    DEG2RAD = PI / 180.0f;
constexpr const float    RAD2DEG = 180.0f / PI;

//=============================================================================
// Core func
//=============================================================================

inline bool Equals(float x, float y, float epsilon = EPSILON) noexcept; // Check whether two floating point values are equal within accuracy.
inline constexpr bool  IsNaN(float value) noexcept; // Check whether a floating point value is NaN.
inline constexpr float ConvertToRadians(float degrees) noexcept;
inline constexpr float ConvertToDegrees(float radians) noexcept;
inline constexpr int   Min(int a, int b) noexcept;
inline constexpr float Min(float a, float b) noexcept;
inline constexpr int   Max(int a, int b) noexcept;
inline constexpr float Max(float a, float b) noexcept;
inline constexpr float Clamp(float value, float min, float max) noexcept;
inline constexpr float Lerp(float a, float b, float f) noexcept; // Linear interpolation between two float values.

//=============================================================================
// Color
//=============================================================================
class Color
{
public:
	constexpr Color() = default;
	constexpr Color(Color&&) = default;
	constexpr Color(const Color&) = default;
	constexpr Color(unsigned uintRGBA);
	constexpr Color(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na = 255) : r(nr), g(ng), b(nb), a(na) {}

	constexpr Color& operator=(Color&&) = default;
	constexpr Color& operator=(const Color&) = default;

	unsigned ToUInt() const { return (unsigned)a << 24 | r << 16 | g << 8 | b; }

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
};

inline bool operator==(const Color& Left, const Color& Right) noexcept;

// Пока не реализованно, при этом функции должны клампится
inline Color operator-(const Color& Left, const Color& Right) noexcept;
inline Color operator+(const Color& Left, const Color& Right) noexcept;
inline Color operator*(const Color& Left, const Color& Right) noexcept;
inline Color& operator-=(Color &Left, const Color& Right) noexcept;
inline Color& operator+=(Color &Left, const Color& Right) noexcept;
inline Color& operator*=(Color &Left, const Color& Right) noexcept;

//=============================================================================
// Point2
//=============================================================================
class Point2
{
public:
	constexpr Point2() = default;
	constexpr Point2(Point2&&) = default;
	constexpr Point2(const Point2&) = default;
	constexpr Point2(int scalar) : x(scalar), y(scalar) {}
	constexpr Point2(int nx, int ny) : x(nx), y(ny) {}

	constexpr Point2& operator=(Point2&&) = default;
	constexpr Point2& operator=(const Point2&) = default;

	int x = 0;
	int y = 0;
};

inline bool operator==(const Point2& Left, const Point2& Right) noexcept;

inline Point2 operator-(const Point2& In) noexcept;
inline Point2 operator-(int Left, const Point2& Right) noexcept;
inline Point2 operator-(const Point2& Left, int Right) noexcept;
inline Point2 operator-(const Point2& Left, const Point2& Right) noexcept;
inline Point2 operator+(int Left, const Point2& Right) noexcept;
inline Point2 operator+(const Point2& Left, int Right) noexcept;
inline Point2 operator+(const Point2& Left, const Point2& Right) noexcept;
inline Point2 operator*(int Left, const Point2& Right) noexcept;
inline Point2 operator*(const Point2& Left, int Right) noexcept;
inline Point2 operator*(const Point2& Left, const Point2& Right) noexcept;
inline Point2 operator/(int Left, const Point2& Right) noexcept;
inline Point2 operator/(const Point2& Left, int Right) noexcept;
inline Point2 operator/(const Point2& Left, const Point2& Right) noexcept;

inline Point2& operator-=(Point2 &Left, int Right) noexcept;
inline Point2& operator-=(Point2 &Left, const Point2& Right) noexcept;
inline Point2& operator+=(Point2 &Left, int Right) noexcept;
inline Point2& operator+=(Point2 &Left, const Point2& Right) noexcept;
inline Point2& operator*=(Point2 &Left, int Right) noexcept;
inline Point2& operator*=(Point2 &Left, const Point2& Right) noexcept;
inline Point2& operator/=(Point2 &Left, int Right) noexcept;
inline Point2& operator/=(Point2 &Left, const Point2& Right) noexcept;

//=============================================================================
// Vector2
//=============================================================================
class Vector2
{
public:
	constexpr Vector2() = default;
	constexpr Vector2(Vector2&&) = default;
	constexpr Vector2(const Vector2&) = default;
	constexpr Vector2(float num) : x(num), y(num) {}
	constexpr Vector2(float nx, float ny) : x(nx), y(ny) {}

	constexpr Vector2& operator=(Vector2&&) = default;
	constexpr Vector2& operator=(const Vector2&) = default;

	float& operator[](size_t i) noexcept { return (&x)[i]; }
	const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector2 GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
};

inline float Distance(const Vector2& v1, const Vector2& v2);
inline float DotProduct(const Vector2& v1, const Vector2& v2);
inline Vector2 Min(const Vector2& v1, const Vector2& v2);
inline Vector2 Max(const Vector2& v1, const Vector2& v2);
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x);
inline Vector2 Mix(const Vector2& u, const Vector2& v, float a);

inline bool operator==(const Vector2& Left, const Vector2& Right) noexcept;

inline Vector2 operator-(const Vector2& In) noexcept;
inline Vector2 operator-(float Left, const Vector2& Right) noexcept;
inline Vector2 operator-(const Vector2& Left, float Right) noexcept;
inline Vector2 operator-(const Vector2& Left, const Vector2& Right) noexcept;
inline Vector2 operator+(float Left, const Vector2& Right) noexcept;
inline Vector2 operator+(const Vector2& Left, float Right) noexcept;
inline Vector2 operator+(const Vector2& Left, const Vector2& Right) noexcept;
inline Vector2 operator*(float Left, const Vector2& Right) noexcept;
inline Vector2 operator*(const Vector2& Left, float Right) noexcept;
inline Vector2 operator*(const Vector2& Left, const Vector2& Right) noexcept;
inline Vector2 operator/(float Left, const Vector2& Right) noexcept;
inline Vector2 operator/(const Vector2& Left, float Right) noexcept;
inline Vector2 operator/(const Vector2& Left, const Vector2& Right) noexcept;

inline Vector2& operator-=(Vector2 &Left, float Right) noexcept;
inline Vector2& operator-=(Vector2 &Left, const Vector2& Right) noexcept;
inline Vector2& operator+=(Vector2 &Left, float Right) noexcept;
inline Vector2& operator+=(Vector2 &Left, const Vector2& Right) noexcept;
inline Vector2& operator*=(Vector2 &Left, float Right) noexcept;
inline Vector2& operator*=(Vector2 &Left, const Vector2& Right) noexcept;
inline Vector2& operator/=(Vector2 &Left, float Right) noexcept;
inline Vector2& operator/=(Vector2 &Left, const Vector2& Right) noexcept;

//=============================================================================
// Impl
//=============================================================================
#include "MicroMath.inl"