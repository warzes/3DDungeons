#pragma once

//=============================================================================
// Core func
//=============================================================================

// Check whether two floating point values are equal within accuracy.
inline bool Equals(float x, float y, float epsilon) noexcept
{
	return fabsf(x - y) < epsilon;
}

// Check whether a floating point value is NaN.
inline constexpr bool IsNaN(float value) noexcept { return value != value; }

inline constexpr float ConvertToRadians(float degrees) noexcept { return degrees * DEG2RAD; }
inline constexpr float ConvertToDegrees(float radians) noexcept { return radians * RAD2DEG; }

inline constexpr int   Min(int a, int b) noexcept { return a < b ? a : b; }
inline constexpr float Min(float a, float b) noexcept { return a < b ? a : b; }
inline constexpr int   Max(int a, int b) noexcept { return a > b ? a : b; }
inline constexpr float Max(float a, float b) noexcept { return a > b ? a : b; }

inline constexpr float Clamp(float value, float min, float max) noexcept
{
	if( value < min ) return min;
	if( value > max ) return max;
	return value;
}

// Linear interpolation between two float values.
inline constexpr float Lerp(float a, float b, float f) noexcept
{
	return a + f * (b - a);
}

//=============================================================================
// Color
//=============================================================================
inline constexpr Color::Color(unsigned uintRGBA)
{
	a = (unsigned char)(uintRGBA >> 24);
	r = (unsigned char)(uintRGBA >> 16);
	g = (unsigned char)(uintRGBA >> 8);
	b = (unsigned char)(uintRGBA >> 0);
}

inline bool operator==(const Color& Left, const Color& Right) noexcept
{
	return Left.r == Right.r 
		&& Left.g == Right.g 
		&& Left.b == Right.b 
		&& Left.a == Right.a; 
}

//=============================================================================
// Point2
//=============================================================================

inline bool operator==(const Point2& Left, const Point2& Right) noexcept { return Left.x == Right.x && Left.y == Right.y; }

inline Point2 operator-(const Point2& In) noexcept                        { return { -In.x, -In.y }; }
inline Point2 operator-(int Left, const Point2& Right) noexcept           { return {   Left - Right.x,   Left - Right.y }; }
inline Point2 operator-(const Point2& Left, int Right) noexcept           { return { Left.x - Right,   Left.y - Right   }; }
inline Point2 operator-(const Point2& Left, const Point2& Right) noexcept { return { Left.x - Right.x, Left.y - Right.y }; }
inline Point2 operator+(int Left, const Point2& Right) noexcept           { return {   Left + Right.x,   Left + Right.y }; }
inline Point2 operator+(const Point2& Left, int Right) noexcept           { return { Left.x + Right,   Left.y + Right   }; }
inline Point2 operator+(const Point2& Left, const Point2& Right) noexcept { return { Left.x + Right.x, Left.y + Right.y }; }
inline Point2 operator*(int Left, const Point2& Right) noexcept           { return {   Left * Right.x,   Left * Right.y }; }
inline Point2 operator*(const Point2& Left, int Right) noexcept           { return { Left.x * Right,   Left.y * Right   }; }
inline Point2 operator*(const Point2& Left, const Point2& Right) noexcept { return { Left.x * Right.x, Left.y * Right.y }; }
inline Point2 operator/(int Left, const Point2& Right) noexcept           { return {   Left / Right.x,   Left / Right.y }; }
inline Point2 operator/(const Point2& Left, int Right) noexcept           { return { Left.x / Right,   Left.y / Right   }; }
inline Point2 operator/(const Point2& Left, const Point2& Right) noexcept { return { Left.x / Right.x, Left.y / Right.y }; }

inline Point2& operator-=(Point2 &Left, int Right) noexcept           { return Left = Left - Right; }
inline Point2& operator-=(Point2 &Left, const Point2& Right) noexcept { return Left = Left - Right; }
inline Point2& operator+=(Point2 &Left, int Right) noexcept           { return Left = Left + Right; }
inline Point2& operator+=(Point2 &Left, const Point2& Right) noexcept { return Left = Left + Right; }
inline Point2& operator*=(Point2 &Left, int Right) noexcept           { return Left = Left * Right; }
inline Point2& operator*=(Point2 &Left, const Point2& Right) noexcept { return Left = Left * Right; }
inline Point2& operator/=(Point2 &Left, int Right) noexcept           { return Left = Left / Right; }
inline Point2& operator/=(Point2 &Left, const Point2& Right) noexcept { return Left = Left / Right; }

//=============================================================================
// Vector2
//=============================================================================

inline float Vector2::GetLength() const { return sqrtf(x * x + y * y); }
inline float Vector2::GetLengthSquared() const { return x * x + y * y; }
inline Vector2 Vector2::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen };
}

inline float Distance(const Vector2& v1, const Vector2& v2) { return (v1 - v2).GetLength(); }
inline float DotProduct(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
inline Vector2 Min(const Vector2& v1, const Vector2& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y) }; }
inline Vector2 Max(const Vector2& v1, const Vector2& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y) }; }
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x) { return a + (b - a) * x; }
inline Vector2 Mix(const Vector2& u, const Vector2& v, float a) { return u * (1.0f - a) + v * a; }

inline bool operator==(const Vector2& Left, const Vector2& Right) noexcept { return Left.x == Right.x && Left.y == Right.y; }

inline Vector2 operator-(const Vector2& In) noexcept                         { return { -In.x, -In.y }; }
inline Vector2 operator-(float Left, const Vector2& Right) noexcept          { return {   Left - Right.x,   Left - Right.y }; }
inline Vector2 operator-(const Vector2& Left, float Right) noexcept          { return { Left.x - Right,   Left.y - Right   }; }
inline Vector2 operator-(const Vector2& Left, const Vector2& Right) noexcept { return { Left.x - Right.x, Left.y - Right.y }; }
inline Vector2 operator+(float Left, const Vector2& Right) noexcept          { return {   Left + Right.x,   Left + Right.y }; }
inline Vector2 operator+(const Vector2& Left, float Right) noexcept          { return { Left.x + Right,   Left.y + Right   }; }
inline Vector2 operator+(const Vector2& Left, const Vector2& Right) noexcept { return { Left.x + Right.x, Left.y + Right.y }; }
inline Vector2 operator*(float Left, const Vector2& Right) noexcept          { return {   Left * Right.x,   Left * Right.y }; }
inline Vector2 operator*(const Vector2& Left, float Right) noexcept          { return { Left.x * Right,   Left.y * Right   }; }
inline Vector2 operator*(const Vector2& Left, const Vector2& Right) noexcept { return { Left.x * Right.x, Left.y * Right.y }; }
inline Vector2 operator/(float Left, const Vector2& Right) noexcept          { return {   Left / Right.x,   Left / Right.y }; }
inline Vector2 operator/(const Vector2& Left, float Right) noexcept          { return { Left.x / Right,   Left.y / Right   }; }
inline Vector2 operator/(const Vector2& Left, const Vector2& Right) noexcept { return { Left.x / Right.x, Left.y / Right.y }; }

inline Vector2& operator-=(Vector2 &Left, float Right) noexcept          { return Left = Left - Right; }
inline Vector2& operator-=(Vector2 &Left, const Vector2& Right) noexcept { return Left = Left - Right; }
inline Vector2& operator+=(Vector2 &Left, float Right) noexcept          { return Left = Left + Right; }
inline Vector2& operator+=(Vector2 &Left, const Vector2& Right) noexcept { return Left = Left + Right; }
inline Vector2& operator*=(Vector2 &Left, float Right) noexcept          { return Left = Left * Right; }
inline Vector2& operator*=(Vector2 &Left, const Vector2& Right) noexcept { return Left = Left * Right; }
inline Vector2& operator/=(Vector2 &Left, float Right) noexcept          { return Left = Left / Right; }
inline Vector2& operator/=(Vector2 &Left, const Vector2& Right) noexcept { return Left = Left / Right; }