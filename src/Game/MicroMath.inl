#pragma once

//=============================================================================
// Core functions Impl
//=============================================================================

// Check whether two floating point values are equal within accuracy.
inline bool Equals(float x, float y, float epsilon) noexcept
{
	return fabsf(x - y) <= epsilon;
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

inline constexpr float Mix(float x, float y, float a) noexcept
{
	return x * (1.0f - a) + y * a;
}

//=============================================================================
// Color Impl
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
// Point2 Impl
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

inline Point2& operator-=(Point2& Left, int Right) noexcept           { return Left = Left - Right; }
inline Point2& operator-=(Point2& Left, const Point2& Right) noexcept { return Left = Left - Right; }
inline Point2& operator+=(Point2& Left, int Right) noexcept           { return Left = Left + Right; }
inline Point2& operator+=(Point2& Left, const Point2& Right) noexcept { return Left = Left + Right; }
inline Point2& operator*=(Point2& Left, int Right) noexcept           { return Left = Left * Right; }
inline Point2& operator*=(Point2& Left, const Point2& Right) noexcept { return Left = Left * Right; }
inline Point2& operator/=(Point2& Left, int Right) noexcept           { return Left = Left / Right; }
inline Point2& operator/=(Point2& Left, const Point2& Right) noexcept { return Left = Left / Right; }

//=============================================================================
// Vector2 Impl
//=============================================================================

inline float Vector2::GetLength() const { return sqrtf(x * x + y * y); }
inline float Vector2::GetLengthSquared() const { return x * x + y * y; }
inline Vector2 Vector2::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen };
}

inline bool Equals(const Vector2& v1, const Vector2& v2, float epsilon) noexcept
{
	if (v1 == v2) return true;
	return ::Equals(v1.x, v2.x, epsilon) 
		&& ::Equals(v1.y, v2.y, epsilon);
}

inline float Distance(const Vector2& v1, const Vector2& v2) { return (v2 - v1).GetLength(); }
inline float DotProduct(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }

inline Vector2 Reflect(const Vector2& v, const Vector2& normal)
{
	return v - normal * DotProduct(normal, v) * 2.0f;
}

inline Vector2 Refract(const Vector2& i, const Vector2& normal, float eta)
{
	const float dotValue = DotProduct(normal, i);
	const float k = 1.0f - eta * eta * (1.0f - dotValue * dotValue);
	return (k >= 0.0f) ? (eta * i - (eta * dotValue + sqrtf(k)) * normal) : Vector2(0.0f);
}

inline Vector2 Min(const Vector2& v1, const Vector2& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y) }; }
inline Vector2 Max(const Vector2& v1, const Vector2& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y) }; }
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x) { return a + (b - a) * x; }
inline Vector2 Mix(const Vector2& x, const Vector2& y, float a) { return x * (1.0f - a) + y * a; }

//=============================================================================
// OLD
inline Vector2 Project(const Vector2& v1, const Vector2& v2)
{
	const float d = DotProduct(v2, v2);
	const float s = DotProduct(v1, v2) / d;
	return { v2.x * s, v2.y * s };
}
inline Vector2 Slide(const Vector2& v, const Vector2& normal)
{
	const float d = DotProduct(v, normal);
	return v - normal * d; // проверить что так { v.x - normal.x * d, v.y - normal.y * d };
}



inline Vector2 Tangent(const Vector2& v)
{
	return { v.y, -v.x };
}

inline Vector2 Rotate(const Vector2& v, float angle)
{
	const float cs = cosf(angle);
	const float sn = sinf(angle);
	return {
		v.x * cs - v.y * sn,
		v.x * sn + v.y * cs
	};
}

inline Vector2 Bezier3(const Vector2& v0, const Vector2& v1, const Vector2& v2, float f)
{
	Vector2 tmp0 = Lerp(v0, v1, f);
	Vector2 tmp1 = Lerp(v1, v2, f);
	return Lerp(tmp0, tmp1, f);
}

inline Vector2 Bezier4(const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3, float f)
{
	Vector2 tmp0 = Lerp(v0, v1, f);
	Vector2 tmp1 = Lerp(v1, v2, f);
	Vector2 tmp2 = Lerp(v2, v3, f);
	Vector2 tmp3 = Lerp(tmp0, tmp1, f);
	Vector2 tmp4 = Lerp(tmp1, tmp2, f);
	return Lerp(tmp3, tmp4, f);
}

inline void Abs(Vector2& v)
{
	v.x = fabsf(v.x);
	v.y = fabsf(v.y);
}

inline void Floor(Vector2& v)
{
	v.x = floorf(v.x);
	v.y = floorf(v.y);
}

inline void Ceil(Vector2& v)
{
	v.x = ceilf(v.x);
	v.y = ceilf(v.y);
}

inline void Round(Vector2& v)
{
	v.x = roundf(v.x);
	v.y = roundf(v.y);
}

inline bool operator==(const Vector2& Left, const Vector2& Right) noexcept   { return Left.x == Right.x && Left.y == Right.y; }
inline bool operator!=(const Vector2& Left, const Vector2& Right) noexcept   { return !(Left == Right); }

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

inline Vector2& operator-=(Vector2& Left, float Right) noexcept          { return Left = Left - Right; }
inline Vector2& operator-=(Vector2& Left, const Vector2& Right) noexcept { return Left = Left - Right; }
inline Vector2& operator+=(Vector2& Left, float Right) noexcept          { return Left = Left + Right; }
inline Vector2& operator+=(Vector2& Left, const Vector2& Right) noexcept { return Left = Left + Right; }
inline Vector2& operator*=(Vector2& Left, float Right) noexcept          { return Left = Left * Right; }
inline Vector2& operator*=(Vector2& Left, const Vector2& Right) noexcept { return Left = Left * Right; }
inline Vector2& operator/=(Vector2& Left, float Right) noexcept          { return Left = Left / Right; }
inline Vector2& operator/=(Vector2& Left, const Vector2& Right) noexcept { return Left = Left / Right; }

//=============================================================================
// Vector3 Impl
//=============================================================================
inline const Vector3 Vector3::Zero    = { 0.0f };
inline const Vector3 Vector3::One     = { 1.0f };
inline const Vector3 Vector3::Left    = {-1.0f, 0.0f, 0.0f };
inline const Vector3 Vector3::Right   = { 1.0f, 0.0f, 0.0f };
inline const Vector3 Vector3::Up      = { 0.0f, 1.0f, 0.0f };
inline const Vector3 Vector3::Down    = { 0.0f,-1.0f, 0.0f };
inline const Vector3 Vector3::Forward = { 0.0f, 0.0f, 1.0f };
inline const Vector3 Vector3::Back    = { 0.0f, 0.0f,-1.0f };

constexpr Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z)
{
}

inline float Vector3::GetLength() const { return sqrtf(x * x + y * y + z * z); }
inline float Vector3::GetLengthSquared() const { return x * x + y * y + z * z; }
inline Vector3 Vector3::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen, z * invLen };
}

inline bool Equals(const Vector3& v1, const Vector3& v2, float epsilon) noexcept
{
	if (v1 == v2) return true;
	return ::Equals(v1.x, v2.x, epsilon)
		&& ::Equals(v1.y, v2.y, epsilon)
		&& ::Equals(v1.z, v2.z, epsilon);
}

inline float Distance(const Vector3& v1, const Vector3& v2) { return (v2 - v1).GetLength(); }
inline float DotProduct(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
{
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

inline Vector3 Reflect(const Vector3& v, const Vector3& normal)
{
	return v - normal * DotProduct(normal, v) * 2.0f;
}

inline Vector3 Refract(const Vector3& i, const Vector3& normal, float eta)
{
	const float dotValue = DotProduct(normal, i);
	const float k = 1.0f - eta * eta * (1.0f - dotValue * dotValue);
	return (k >= 0.0f) ? (eta * i - (eta * dotValue + sqrtf(k)) * normal) : Vector3(0.0f);
}

inline Vector3 Min(const Vector3& v1, const Vector3& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z) }; }
inline Vector3 Max(const Vector3& v1, const Vector3& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z) }; }
inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x) { return a + (b - a) * x; }
inline Vector3 Mix(const Vector3& x, const Vector3& y, float t) { return x * (1.0f - t) + y * t; }

//=============================================================================
// OLD
inline Vector3 Project(const Vector3& v0, const Vector3& v1)
{
	const float d = DotProduct(v1, v1);
	const float s = DotProduct(v0, v1) / d;
	return { v1.x * s, v1.y * s, v1.z * s };
}

inline float Angle(const Vector3& v1, const Vector3& v2)
{
	float x = DotProduct(v1, v2) / (v1.GetLength() * v2.GetLength());
	return RAD2DEG * acosf(Clamp(x, -1.0f, 1.0f)); // TODO: не конвертировать в градусы
}

inline Vector3 Slide(const Vector3& v, const Vector3& normal)
{
	const float d = DotProduct(v, normal);
	return v - normal * d; //проверить что так { v.x - normal.x * d, v.y - normal.y * d, v.z - normal.z * d };
}

inline Vector3 Rotate(const Vector3& v0, Vector3 ra, float angle)
{
	const float cs = cosf(angle);
	const float sn = sinf(angle);
	ra = ra.GetNormalize();
	return {
		v0.x * (cs + ra.x * ra.x * (1.0f - cs)) + v0.y * (ra.x * ra.y * (1.0f - cs) - ra.z * sn) + v0.z * (ra.x * ra.z * (1.0f - cs) + ra.y * sn),
		v0.x * (ra.y * ra.x * (1.0f - cs) + ra.z * sn) + v0.y * (cs + ra.y * ra.y * (1.0f - cs)) + v0.z * (ra.y * ra.z * (1.0f - cs) - ra.x * sn),
		v0.x * (ra.z * ra.x * (1.0f - cs) - ra.y * sn) + v0.y * (ra.z * ra.y * (1.0f - cs) + ra.x * sn) + v0.z * (cs + ra.z * ra.z * (1.0f - cs))
	};
}

inline Vector3 Bezier3(const Vector3& v0, const Vector3& v1, const Vector3& v2, float f)
{
	Vector3 tmp0 = Lerp(v0, v1, f);
	Vector3 tmp1 = Lerp(v1, v2, f);
	return Lerp(tmp0, tmp1, f);
}

inline Vector3 Bezier4(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, float f)
{
	Vector3 tmp0 = Lerp(v0, v1, f);
	Vector3 tmp1 = Lerp(v1, v2, f);
	Vector3 tmp2 = Lerp(v2, v3, f);
	Vector3 tmp3 = Lerp(tmp0, tmp1, f);
	Vector3 tmp4 = Lerp(tmp1, tmp2, f);
	return Lerp(tmp3, tmp4, f);
}

inline void Abs(Vector3& v)
{
	v.x = fabsf(v.x);
	v.y = fabsf(v.y);
	v.z = fabsf(v.z);
}

inline void Floor(Vector3& v)
{
	v.x = floorf(v.x);
	v.y = floorf(v.y);
	v.z = floorf(v.z);
}

inline void Ceil(Vector3& v)
{
	v.x = ceilf(v.x);
	v.y = ceilf(v.y);
	v.z = ceilf(v.z);
}

inline void Round(Vector3& v)
{
	v.x = roundf(v.x);
	v.y = roundf(v.y);
	v.z = roundf(v.z);
}

inline bool operator==(const Vector3& Left, const Vector3& Right) noexcept   { return Left.x == Right.x && Left.y == Right.y && Left.z == Right.z; }
inline bool operator!=(const Vector3& Left, const Vector3& Right) noexcept   { return !(Left == Right); }

inline Vector3 operator-(const Vector3& In) noexcept                         { return { -In.x, -In.y, -In.z }; }
inline Vector3 operator-(float Left, const Vector3& Right) noexcept          { return {   Left - Right.x,   Left - Right.y,   Left - Right.z }; }
inline Vector3 operator-(const Vector3& Left, float Right) noexcept          { return { Left.x - Right,   Left.y - Right,   Left.z - Right   }; }
inline Vector3 operator-(const Vector3& Left, const Vector3& Right) noexcept { return { Left.x - Right.x, Left.y - Right.y, Left.z - Right.z }; }
inline Vector3 operator+(float Left, const Vector3& Right) noexcept          { return {   Left + Right.x,   Left + Right.y,   Left + Right.z }; }
inline Vector3 operator+(const Vector3& Left, float Right) noexcept          { return { Left.x + Right,   Left.y + Right,   Left.z + Right   }; }
inline Vector3 operator+(const Vector3& Left, const Vector3& Right) noexcept { return { Left.x + Right.x, Left.y + Right.y, Left.z + Right.z }; }
inline Vector3 operator*(float Left, const Vector3& Right) noexcept          { return {   Left * Right.x,   Left * Right.y,   Left * Right.z }; }
inline Vector3 operator*(const Vector3& Left, float Right) noexcept          { return { Left.x * Right,   Left.y * Right,   Left.z * Right   }; }
inline Vector3 operator*(const Vector3& Left, const Vector3& Right) noexcept { return { Left.x * Right.x, Left.y * Right.y, Left.z * Right.z }; }

inline Vector3 operator*(const Quaternion& Left, const Vector3& Right) noexcept
{
	const Vector3 QuatVector(Left.x, Left.y, Left.z);
	const Vector3 uv(CrossProduct(QuatVector, Right));
	const Vector3 uuv(CrossProduct(QuatVector, uv));

	return Right + ((uv * Left.w) + uuv) * 2.0f;
}

inline Vector3 operator*(const Vector3& Left, const Quaternion& Right) noexcept
{
	return Right.Inverse() * Left;
}

inline Vector3 operator*(const Matrix3& Left, const Vector3& Right) noexcept
{
	return {
		Left[0][0] * Right.x + Left[1][0] * Right.y + Left[2][0] * Right.z,
		Left[0][1] * Right.x + Left[1][1] * Right.y + Left[2][1] * Right.z,
		Left[0][2] * Right.x + Left[1][2] * Right.y + Left[2][2] * Right.z
	};
}

inline Vector3 operator*(const Vector3& Left, const Matrix3& Right) noexcept
{
	return {
		Right[0][0] * Left.x + Right[0][1] * Left.y + Right[0][2] * Left.z,
		Right[1][0] * Left.x + Right[1][1] * Left.y + Right[1][2] * Left.z,
		Right[2][0] * Left.x + Right[2][1] * Left.y + Right[2][2] * Left.z
	};
}

inline Vector3 operator/(float Left, const Vector3& Right) noexcept          { return {   Left / Right.x,   Left / Right.y,   Left / Right.z }; }
inline Vector3 operator/(const Vector3& Left, float Right) noexcept          { return { Left.x / Right,   Left.y / Right,   Left.z / Right   }; }
inline Vector3 operator/(const Vector3& Left, const Vector3& Right) noexcept { return { Left.x / Right.x, Left.y / Right.y, Left.z / Right.z }; }

inline Vector3 operator/(const Matrix3& Left, const Vector3& Right) noexcept
{
	return Left.Inverse() * Right;
}

inline Vector3 operator/(const Vector3& Left, const Matrix3& Right) noexcept
{
	return Left * Right.Inverse();
}

inline Vector3& operator-=(Vector3& Left, float Right) noexcept { return Left = Left - Right; }
inline Vector3& operator-=(Vector3& Left, const Vector3& Right) noexcept { return Left = Left - Right; }
inline Vector3& operator+=(Vector3& Left, float Right) noexcept { return Left = Left + Right; }
inline Vector3& operator+=(Vector3& Left, const Vector3& Right) noexcept { return Left = Left + Right; }
inline Vector3& operator*=(Vector3& Left, float Right) noexcept { return Left = Left * Right; }
inline Vector3& operator*=(Vector3& Left, const Vector3& Right) noexcept { return Left = Left * Right; }
inline Vector3& operator/=(Vector3& Left, float Right) noexcept { return Left = Left / Right; }
inline Vector3& operator/=(Vector3& Left, const Vector3& Right) noexcept { return Left = Left / Right; }

//=============================================================================
// Vector4 Impl
//=============================================================================

inline float Vector4::GetLength() const { return sqrtf(x * x + y * y + z * z + w * w); }
inline float Vector4::GetLengthSquared() const { return x * x + y * y + z * z + w * w; }
inline Vector4 Vector4::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen, z * invLen, w * invLen };
}

inline bool Equals(const Vector4& v1, const Vector4& v2, float epsilon) noexcept
{
	if (v1 == v2) return true;
	return ::Equals(v1.x, v2.x, epsilon)
		&& ::Equals(v1.y, v2.y, epsilon)
		&& ::Equals(v1.z, v2.z, epsilon)
		&& ::Equals(v1.w, v2.w, epsilon);
}

inline float DotProduct(const Vector4& v1, const Vector4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline Vector4 Min(const Vector4& v1, const Vector4& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z), Min(v1.w, v2.w) }; }
inline Vector4 Max(const Vector4& v1, const Vector4& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z), Max(v1.w, v2.w) }; }
inline Vector4 Lerp(const Vector4& a, const Vector4& b, float x) { return a + (b - a) * x; }
inline Vector4 Mix(const Vector4& x, const Vector4& y, float a) { return x * (1.0f - a) + y * a; }

//=============================================================================
// OLD
inline void Abs(Vector4& v)
{
	v.x = fabsf(v.x);
	v.y = fabsf(v.y);
	v.z = fabsf(v.z);
	v.w = fabsf(v.w);
}

inline void Floor(Vector4& v)
{
	v.x = floorf(v.x);
	v.y = floorf(v.y);
	v.z = floorf(v.z);
	v.w = floorf(v.w);
}

inline void Ceil(Vector4& v)
{
	v.x = ceilf(v.x);
	v.y = ceilf(v.y);
	v.z = ceilf(v.z);
	v.w = ceilf(v.w);
}

inline void Round(Vector4& v)
{
	v.x = roundf(v.x);
	v.y = roundf(v.y);
	v.z = roundf(v.z);
	v.w = roundf(v.w);
}

inline bool operator==(const Vector4& Left, const Vector4& Right) noexcept   { return Left.x == Right.x && Left.y == Right.y && Left.z == Right.z && Left.w == Right.w; }
inline bool operator!=(const Vector4& Left, const Vector4& Right) noexcept   { return !(Left == Right); }

inline Vector4 operator-(const Vector4& In) noexcept                         { return { -In.x, -In.y, -In.z, -In.w }; }
inline Vector4 operator-(float Left, const Vector4& Right) noexcept          { return {   Left - Right.x,   Left - Right.y,   Left - Right.z,   Left - Right.w }; }
inline Vector4 operator-(const Vector4& Left, float Right) noexcept          { return { Left.x - Right,   Left.y - Right,   Left.z - Right,   Left.w - Right   }; }
inline Vector4 operator-(const Vector4& Left, const Vector4& Right) noexcept { return { Left.x - Right.x, Left.y - Right.y, Left.z - Right.z, Left.w - Right.w }; }
inline Vector4 operator+(float Left, const Vector4& Right) noexcept          { return {   Left + Right.x,   Left + Right.y,   Left + Right.z,   Left + Right.w }; }
inline Vector4 operator+(const Vector4& Left, float Right) noexcept          { return { Left.x + Right,   Left.y + Right,   Left.z + Right,   Left.w + Right   }; }
inline Vector4 operator+(const Vector4& Left, const Vector4& Right) noexcept { return { Left.x + Right.x, Left.y + Right.y, Left.z + Right.z, Left.w + Right.w }; }
inline Vector4 operator*(float Left, const Vector4& Right) noexcept          { return {   Left * Right.x,   Left * Right.y,   Left * Right.z,   Left * Right.w }; }
inline Vector4 operator*(const Vector4& Left, float Right) noexcept          { return { Left.x * Right,   Left.y * Right,   Left.z * Right,   Left.w * Right   }; }
inline Vector4 operator*(const Vector4& Left, const Vector4& Right) noexcept { return { Left.x * Right.x, Left.y * Right.y, Left.z * Right.z, Left.w * Right.w }; }

inline Vector4 operator*(const Quaternion& Left, const Vector4& Right) noexcept
{
	return Vector4(Left * Vector3(Right.x, Right.y, Right.z), Right.w);
}

inline Vector4 operator*(const Vector4& Left, const Quaternion& Right) noexcept
{
	return Right.Inverse() * Left;
}

inline Vector4 operator*(const Matrix4& Left, const Vector4& Right) noexcept
{
	return {
		Left[0][0] * Right[0] + Left[1][0] * Right[1] + Left[2][0] * Right[2] + Left[3][0] * Right[3],
		Left[0][1] * Right[0] + Left[1][1] * Right[1] + Left[2][1] * Right[2] + Left[3][1] * Right[3],
		Left[0][2] * Right[0] + Left[1][2] * Right[1] + Left[2][2] * Right[2] + Left[3][2] * Right[3],
		Left[0][3] * Right[0] + Left[1][3] * Right[1] + Left[2][3] * Right[2] + Left[3][3] * Right[3] };
}
inline Vector4 operator*(const Vector4& Left, const Matrix4& Right) noexcept
{
	return {
		Right[0][0] * Left[0] + Right[0][1] * Left[1] + Right[0][2] * Left[2] + Right[0][3] * Left[3],
		Right[1][0] * Left[0] + Right[1][1] * Left[1] + Right[1][2] * Left[2] + Right[1][3] * Left[3],
		Right[2][0] * Left[0] + Right[2][1] * Left[1] + Right[2][2] * Left[2] + Right[2][3] * Left[3],
		Right[3][0] * Left[0] + Right[3][1] * Left[1] + Right[3][2] * Left[2] + Right[3][3] * Left[3] };
}

inline Vector4 operator/(float Left, const Vector4& Right) noexcept          { return {   Left / Right.x,   Left / Right.y,   Left / Right.z,   Left / Right.w }; }
inline Vector4 operator/(const Vector4& Left, float Right) noexcept          { return { Left.x / Right,   Left.y / Right,   Left.z / Right,   Left.w / Right   }; }
inline Vector4 operator/(const Vector4& Left, const Vector4& Right) noexcept { return { Left.x / Right.x, Left.y / Right.y, Left.z / Right.z, Left.w / Right.w }; }

inline Vector4 operator/(const Matrix4& Left, const Vector4& Right) noexcept
{
	return Left.Inverse() * Right;
}

inline Vector4 operator/(const Vector4& Left, const Matrix4& Right) noexcept
{
	return Left * Right.Inverse();
}

inline Vector4& operator-=(Vector4& Left, float Right) noexcept { return Left = Left - Right; }
inline Vector4& operator-=(Vector4& Left, const Vector4& Right) noexcept { return Left = Left - Right; }
inline Vector4& operator+=(Vector4& Left, float Right) noexcept { return Left = Left + Right; }
inline Vector4& operator+=(Vector4& Left, const Vector4& Right) noexcept { return Left = Left + Right; }
inline Vector4& operator*=(Vector4& Left, float Right) noexcept { return Left = Left * Right; }
inline Vector4& operator*=(Vector4& Left, const Vector4& Right) noexcept { return Left = Left * Right; }
inline Vector4& operator/=(Vector4& Left, float Right) noexcept { return Left = Left / Right; }
inline Vector4& operator/=(Vector4& Left, const Vector4& Right) noexcept { return Left = Left / Right; }

//=============================================================================
// Quaternion Impl
//=============================================================================

const inline Quaternion Quaternion::Identity = { 1.0f, 0.0f, 0.0f, 0.0f };

inline Quaternion::Quaternion(const Vector3& u, const Vector3& v)
{
	// http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	const float normUnormV = sqrtf(DotProduct(u, u) * DotProduct(v, v));
	float realPart = normUnormV + DotProduct(u, v);
	Vector3 t;
	if (realPart < 1.e-6f * normUnormV)
	{
		// If u and v are exactly opposite, rotate 180 degrees around an arbitrary orthogonal axis. Axis normalisation can happen later, when we normalise the quaternion.
		realPart = 0.0f;
		t = fabsf(u.x) > fabsf(u.z) ? Vector3(-u.y, u.x, 0.0f) : Vector3(0.0f, -u.z, u.y);
	}
	else
	{
		// Otherwise, build quaternion the standard way.
		t = CrossProduct(u, v);
	}
	*this = Quaternion(realPart, t.x, t.y, t.z).GetNormalize();
}

inline Quaternion::Quaternion(const Vector3& eulerAngle)
{
	const float x_ = eulerAngle.x * 0.5f;
	const float y_ = eulerAngle.y * 0.5f;
	const float z_ = eulerAngle.z * 0.5f;
	const float cosX = cosf(x_);
	const float cosY = cosf(y_);
	const float cosZ = cosf(z_);
	const float sinX = sinf(x_);
	const float sinY = sinf(y_);
	const float sinZ = sinf(z_);

	w = cosX * cosY * cosZ + sinX * sinY * sinZ;
	x = sinX * cosY * cosZ - cosX * sinY * sinZ;
	y = cosX * sinY * cosZ + sinX * cosY * sinZ;
	z = cosX * cosY * sinZ - sinX * sinY * cosZ;
}

inline Quaternion::Quaternion(const Matrix3& m)
{
	*this = CastToQuaternion(m);
}

inline Quaternion::Quaternion(const Matrix4& m)
{
	*this = CastToQuaternion(m);
}

inline Quaternion::operator Matrix3() const
{
	return CastToMatrix3(*this);
}

inline Quaternion::operator Matrix4() const
{
	return CastToMatrix4(*this);
}

inline float Quaternion::GetLength() const { return sqrt(w * w + x * x + y * y + z * z); }
inline float Quaternion::GetLengthSquared() const { return w * w + x * x + y * y + z * z; }

inline Quaternion Quaternion::GetNormalize() const
{
	const float len = GetLength();
	if (len <= 0.0f) return Quaternion::Identity;
	const float oneOverLen = 1.0f / len;
	return { w * oneOverLen, x * oneOverLen, y * oneOverLen, z * oneOverLen };
}

inline Quaternion Quaternion::Inverse() const
{
	return Conjugate() / DotProduct(*this, *this);
}

inline float Quaternion::GetAngle() const
{
	if (fabsf(w) > CosOneOverTwo)
	{
		const float a = asin(sqrt(x * x + y * y + z * z)) * 2.0f;
		if (w < 0.0f)
			return PI * 2.0f - a;
		return a;
	}

	return acos(w) * 2.0f;
}

inline Vector3 Quaternion::GetAxis() const
{
	const float tmp1 = 1.0f - w * w;
	if (tmp1 <= 0.0f)
		return { 0.0f, 0.0f, 1.0f };
	const float tmp2 = 1.0f / sqrt(tmp1);
	return { x * tmp2, y * tmp2, z * tmp2 };
}

inline Vector3 Quaternion::EulerAngles() const
{
	return Vector3(PitchAngle(), YawAngle(), RollAngle());
}

inline float Quaternion::YawAngle() const
{
	return asin(Clamp(-2.0f * (x * z - w * y), -1.0f, 1.0f));
}

inline float Quaternion::PitchAngle() const
{
	//return (atan(2 * (y * z + w * x), w * w - x * x - y * y + z * z));
	const float y = 2.0f * (y * z + w * x);
	const float x = w * w - x * x - y * y + z * z;

	if (Equals(Vector2(x, y), Vector2(0.0f), std::numeric_limits<float>::epsilon())) //avoid atan2(0,0) - handle singularity - Matiis
		return 2.0f * atan2(x, w);

	return atan2(y, x);
}

inline float Quaternion::RollAngle() const
{
	const float y = 2.0f * (x * y + w * z);
	const float x = w * w + x * x - y * y - z * z;

	if (Equals(Vector2(x, y), Vector2(0.0f), std::numeric_limits<float>::epsilon())) //avoid atan2(0,0) - handle singularity - Matiis
		return 0.0f;

	return atan2(y, x);
}

inline bool Equals(const Quaternion& v1, const Quaternion& v2, float epsilon) noexcept
{
	if (v1 == v2) return true;
	return ::Equals(v1.w, v2.w, epsilon)
		&& ::Equals(v1.x, v2.x, epsilon)
		&& ::Equals(v1.y, v2.y, epsilon)
		&& ::Equals(v1.z, v2.z, epsilon);
}

inline Quaternion CastToQuaternion(const Matrix3& m)
{
	const float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	const float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	const float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
	const float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	const float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	const float mult = 0.25f / biggestVal;

	switch (biggestIndex)
	{
	case 0: return {biggestVal, (m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult};
	case 1: return {(m[1][2] - m[2][1]) * mult, biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult};
	case 2: return {(m[2][0] - m[0][2]) * mult, (m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult};
	case 3: return {(m[0][1] - m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal};
	default:
		assert(false);
		return Quaternion::Identity;
	}
}

inline Quaternion CastToQuaternion(const Matrix4& m)
{
	return CastToQuaternion(Matrix3(m));
}

inline float DotProduct(const Quaternion& q1, const Quaternion& q2) { return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z; }

inline Quaternion CrossProduct(const Quaternion& q1, const Quaternion& q2)
{
	return {
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x};
}

inline Quaternion Rotate(const Quaternion& q, float angle, const Vector3& axis)
{
	Vector3 tmp = axis;

	// Axis of rotation must be normalised
	const float len = tmp.GetLength();
	if (fabsf(len - 1.0f) > 0.001f)
	{
		const float oneOverLen = 1.0f / len;
		tmp.x *= oneOverLen;
		tmp.y *= oneOverLen;
		tmp.z *= oneOverLen;
	}

	const float halfAndgle = angle * 0.5f;
	const float Sin = sin(halfAndgle);
	return q * Quaternion(cos(halfAndgle), tmp.x * Sin, tmp.y * Sin, tmp.z * Sin);
}

inline Quaternion AngleAxis(float angle, const Vector3& axis)
{
	const float halfAndgle = angle * 0.5f;
	const float Sin = sin(halfAndgle);
	return Quaternion(cos(halfAndgle), axis * Sin);
}

inline Quaternion QuatLookAt(const Vector3& direction, const Vector3& up)
{
	Matrix3 Result;
	Result[2] = direction;
	const Vector3& Right = CrossProduct(up, Result[2]);
	Result[0] = Right * 1.0f / sqrt(Max(0.00001f, DotProduct(Right, Right)));
	Result[1] = CrossProduct(Result[2], Result[0]);

	return CastToQuaternion(Result);
}

inline Quaternion Mix(const Quaternion& x, const Quaternion& y, float a)
{
	const float cosTheta = DotProduct(x, y);
	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if (cosTheta > 1.0f - std::numeric_limits<float>::epsilon())
	{
		// Linear interpolation
		return {
			Mix(x.w, y.w, a),
			Mix(x.x, y.x, a),
			Mix(x.y, y.y, a),
			Mix(x.z, y.z, a) };
	}
	else
	{
		const float angle = acosf(cosTheta);
		return (sin((1.0f - a) * angle) * x + sin(a * angle) * y) / sin(angle);
	}
}

inline Quaternion Lerp(const Quaternion& x, const Quaternion& y, float a)
{
	// Lerp is only defined in [0, 1]
	assert(a >= 0.0f);
	assert(a <= 1.0f);
	return x * (1.0f - a) + (y * a);
}

inline Quaternion SLerp(const Quaternion& x, const Quaternion& y, float a)
{
	Quaternion z = y;

	float cosTheta = DotProduct(x, y);

	// If cosTheta < 0, the interpolation will take the long way around the sphere.
	// To fix this, one quat must be negated.
	if (cosTheta < 0.0f)
	{
		z = -y;
		cosTheta = -cosTheta;
	}

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if (cosTheta > 1.0f - std::numeric_limits<float>::epsilon())
	{
		// Linear interpolation
		return { 
			Mix(x.w, z.w, a),
			Mix(x.x, z.x, a),
			Mix(x.y, z.y, a),
			Mix(x.z, z.z, a) };
	}
	else
	{
		const float angle = acos(cosTheta);
		return (sin((1.0f - a) * angle) * x + sin(a * angle) * z) / sin(angle);
	}
}

inline Quaternion SLerp(const Quaternion& x, const Quaternion& y, float a, float k)
{
	Quaternion z = y;

	float cosTheta = DotProduct(x, y);

	// If cosTheta < 0, the interpolation will take the long way around the sphere.
	// To fix this, one quat must be negated.
	if (cosTheta < 0.0f)
	{
		z = -y;
		cosTheta = -cosTheta;
	}

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if (cosTheta > 1.0f - std::numeric_limits<float>::epsilon())
	{
		// Linear interpolation
		return {
			Mix(x.w, z.w, a),
			Mix(x.x, z.x, a),
			Mix(x.y, z.y, a),
			Mix(x.z, z.z, a) };
	}
	else
	{
		// Graphics Gems III, page 96
		const float angle = acos(cosTheta);
		const float phi = angle + k * PI;
		return (sin(angle - a * phi) * x + sin(a * phi) * z) / sin(angle);
	}
}

inline bool operator==(const Quaternion& Left, const Quaternion& Right) noexcept { return Left.w == Right.w && Left.x == Right.x && Left.y == Right.y && Left.z == Right.z; }
inline bool operator!=(const Quaternion& Left, const Quaternion& Right) noexcept { return !(Left == Right); }

inline Quaternion operator-(const Quaternion& q) noexcept { return { -q.w , -q.x, -q.y, -q.z }; }
inline Quaternion operator-(const Quaternion& Left, const Quaternion& Right) noexcept { return { Left.w - Right.w, Left.x - Right.x, Left.y - Right.y, Left.z - Right.z }; }
inline Quaternion operator+(const Quaternion& Left, const Quaternion& Right) noexcept { return { Left.w + Right.w, Left.x + Right.x, Left.y + Right.y, Left.z + Right.z }; }
inline Quaternion operator*(float Left, const Quaternion& Right) noexcept { return { Left * Right.w, Left * Right.x, Left * Right.y, Left * Right.z }; }
inline Quaternion operator*(const Quaternion& Left, float Right) noexcept { return { Left.w * Right, Left.x * Right, Left.y * Right, Left.z * Right }; }
inline Quaternion operator*(const Quaternion& Left, const Quaternion& Right) noexcept
{
	return {
		Left.w * Right.w - Left.x * Right.x - Left.y * Right.y - Left.z * Right.z,
		Left.w * Right.x + Left.x * Right.w + Left.y * Right.z - Left.z * Right.y,
		Left.w * Right.y + Left.y * Right.w + Left.z * Right.x - Left.x * Right.z,
		Left.w * Right.z + Left.z * Right.w + Left.x * Right.y - Left.y * Right.x
	};
}
inline Quaternion operator/(const Quaternion& Left, float Right) noexcept { return { Left.w / Right, Left.x / Right, Left.y / Right, Left.z / Right }; }

inline Quaternion& operator-=(Quaternion& Left, const Quaternion& Right) noexcept { return Left = Left - Right; }
inline Quaternion& operator+=(Quaternion& Left, const Quaternion& Right) noexcept { return Left = Left + Right; }
inline Quaternion& operator*=(Quaternion& Left, float Right) noexcept { return Left = Left * Right; }
inline Quaternion& operator*=(Quaternion& Left, const Quaternion& Right) noexcept { return Left = Left * Right; }
inline Quaternion& operator/=(Quaternion& Left, float Right) noexcept { return Left = Left / Right; }


//=============================================================================
// OLD

//inline Quaternion::Quaternion(float angle, const Vector3& axis)
//{
//	FromAngleAxis(angle, axis);
//}
inline void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
{
	Vector3 tmp = axis;

	// Axis of rotation must be normalised
	const float len = tmp.GetLength();
	if (fabsf(len - 1.0f) > 0.001f)
	{
		const float oneOverLen = 1.0f / len;
		tmp.x *= oneOverLen;
		tmp.y *= oneOverLen;
		tmp.z *= oneOverLen;
	}
	const float half = angle * 0.5f;
	const float sinAngle = sinf(half);
	x = tmp.x * sinAngle;
	y = tmp.y * sinAngle;
	z = tmp.z * sinAngle;
	w = cosf(half);
}

inline void Quaternion::FromMatrix(const Matrix4_old& m0)
{
	float scale = m0[0] + m0[5] + m0[10];
	if( scale > 0.0f )
	{
		float sr = sqrtf(scale + 1.0f);
		w = sr * 0.5f;
		sr = 0.5f / sr;
		x = (m0[9] - m0[6]) * sr;
		y = (m0[2] - m0[8]) * sr;
		z = (m0[4] - m0[1]) * sr;
	}
	else if( (m0[0] >= m0[5]) && (m0[0] >= m0[10]) )
	{
		float sr = sqrtf(1.0f + m0[0] - m0[5] - m0[10]);
		float half = 0.5f / sr;
		x = 0.5f * sr;
		y = (m0[4] + m0[1]) * half;
		z = (m0[8] + m0[2]) * half;
		w = (m0[9] - m0[6]) * half;
	}
	else if( m0[5] > m0[10] )
	{
		float sr = sqrtf(1.0f + m0[5] - m0[0] - m0[10]);
		float half = 0.5f / sr;
		x = (m0[1] + m0[4]) * half;
		y = 0.5f * sr;
		z = (m0[6] + m0[9]) * half;
		w = (m0[2] - m0[8]) * half;
	}
	else
	{
		float sr = sqrtf(1.0f + m0[10] - m0[0] - m0[5]);
		float half = 0.5f / sr;
		x = (m0[2] + m0[8]) * half;
		y = (m0[6] + m0[9]) * half;
		z = 0.5f * sr;
		w = (m0[4] - m0[1]) * half;
	}
}

//=============================================================================
// Matrix3 Impl
//=============================================================================

const inline Matrix3 Matrix3::Identity = { 
	1.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

inline constexpr Matrix3::Matrix3(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2) 
	: value{ {x0, y0, z0}, {x1, y1, z1}, {x2, y2, z2} }
{
}

inline Matrix3::Matrix3(const Matrix4& m) : value{ m[0], m[1], m[2] }
{
}

inline float Matrix3::GetDeterminant() const
{
	return
		+ value[0][0] * (value[1][1] * value[2][2] - value[2][1] * value[1][2])
		- value[1][0] * (value[0][1] * value[2][2] - value[2][1] * value[0][2])
		+ value[2][0] * (value[0][1] * value[1][2] - value[1][1] * value[0][2]);
}

inline Matrix3 Matrix3::Inverse() const
{
	const float OneOverDeterminant = 1.0f / (
		+ value[0][0] * (value[1][1] * value[2][2] - value[2][1] * value[1][2])
		- value[1][0] * (value[0][1] * value[2][2] - value[2][1] * value[0][2])
		+ value[2][0] * (value[0][1] * value[1][2] - value[1][1] * value[0][2]));
	Matrix3 Inverse;
	Inverse[0][0] = +(value[1][1] * value[2][2] - value[2][1] * value[1][2]) * OneOverDeterminant;
	Inverse[1][0] = -(value[1][0] * value[2][2] - value[2][0] * value[1][2]) * OneOverDeterminant;
	Inverse[2][0] = +(value[1][0] * value[2][1] - value[2][0] * value[1][1]) * OneOverDeterminant;
	Inverse[0][1] = -(value[0][1] * value[2][2] - value[2][1] * value[0][2]) * OneOverDeterminant;
	Inverse[1][1] = +(value[0][0] * value[2][2] - value[2][0] * value[0][2]) * OneOverDeterminant;
	Inverse[2][1] = -(value[0][0] * value[2][1] - value[2][0] * value[0][1]) * OneOverDeterminant;
	Inverse[0][2] = +(value[0][1] * value[1][2] - value[1][1] * value[0][2]) * OneOverDeterminant;
	Inverse[1][2] = -(value[0][0] * value[1][2] - value[1][0] * value[0][2]) * OneOverDeterminant;
	Inverse[2][2] = +(value[0][0] * value[1][1] - value[1][0] * value[0][1]) * OneOverDeterminant;
	return Inverse;
}

inline Matrix3 Matrix3::Transpose() const
{
	Matrix3 Result;
	Result[0][0] = value[0][0];
	Result[0][1] = value[1][0];
	Result[0][2] = value[2][0];

	Result[1][0] = value[0][1];
	Result[1][1] = value[1][1];
	Result[1][2] = value[2][1];

	Result[2][0] = value[0][2];
	Result[2][1] = value[1][2];
	Result[2][2] = value[2][2];
	return Result;
}

inline Matrix3 CastToMatrix3(const Quaternion& q)
{
	Matrix3 Result = Matrix3::Identity;
	const float qxx(q.x * q.x);
	const float qyy(q.y * q.y);
	const float qzz(q.z * q.z);
	const float qxz(q.x * q.z);
	const float qxy(q.x * q.y);
	const float qyz(q.y * q.z);
	const float qwx(q.w * q.x);
	const float qwy(q.w * q.y);
	const float qwz(q.w * q.z);

	Result[0][0] = 1.0f - 2.0f * (qyy + qzz);
	Result[0][1] = 2.0f * (qxy + qwz);
	Result[0][2] = 2.0f * (qxz - qwy);

	Result[1][0] = 2.0f * (qxy - qwz);
	Result[1][1] = 1.0f - 2.0f * (qxx + qzz);
	Result[1][2] = 2.0f * (qyz + qwx);

	Result[2][0] = 2.0f * (qxz + qwy);
	Result[2][1] = 2.0f * (qyz - qwx);
	Result[2][2] = 1.0f - 2.0f * (qxx + qyy);
	return Result;
}

inline bool operator==(const Matrix3& Left, const Matrix3& Right) noexcept
{
	return (Left[0] == Right[0] && Left[1] == Right[1] && Left[2] == Right[2]);
}

inline bool operator!=(const Matrix3& Left, const Matrix3& Right) noexcept
{
	return !(Left == Right);
}

inline Matrix3 operator-(const Matrix3& m) noexcept
{
	return {-m[0], -m[1], -m[2] };
}

inline Matrix3 operator-(float Left, const Matrix3& Right) noexcept
{
	return { Left - Right[0], Left - Right[1], Left - Right[2] };
}

inline Matrix3 operator-(const Matrix3& Left, float Right) noexcept
{
	return { Left[0] - Right, Left[1] - Right, Left[2] - Right };
}

inline Matrix3 operator-(const Matrix3& Left, const Matrix3& Right) noexcept
{
	return { Left[0] - Right[0], Left[1] - Right[1], Left[2] - Right[2] };
}

inline Matrix3 operator+(float Left, const Matrix3& Right) noexcept
{
	return Right + Left;
}

inline Matrix3 operator+(const Matrix3& Left, float Right) noexcept
{
	return { Left[0] + Right, Left[1] + Right, Left[2] + Right };
}

inline Matrix3 operator+(const Matrix3& Left, const Matrix3& Right) noexcept
{
	return { Left[0] + Right[0], Left[1] + Right[1], Left[2] + Right[2] };
}

inline Matrix3 operator*(float Left, const Matrix3& Right) noexcept
{
	return Right * Left;
}

inline Matrix3 operator*(const Matrix3& Left, float Right) noexcept
{
	return { Left[0] * Right, Left[1] * Right, Left[2] * Right };
}

inline Matrix3 operator*(const Matrix3& Left, const Matrix3& Right) noexcept
{
	// TODO: убрать лишние операции
	const float SrcA00 = Left[0][0];
	const float SrcA01 = Left[0][1];
	const float SrcA02 = Left[0][2];
	const float SrcA10 = Left[1][0];
	const float SrcA11 = Left[1][1];
	const float SrcA12 = Left[1][2];
	const float SrcA20 = Left[2][0];
	const float SrcA21 = Left[2][1];
	const float SrcA22 = Left[2][2];

	const float SrcB00 = Right[0][0];
	const float SrcB01 = Right[0][1];
	const float SrcB02 = Right[0][2];
	const float SrcB10 = Right[1][0];
	const float SrcB11 = Right[1][1];
	const float SrcB12 = Right[1][2];
	const float SrcB20 = Right[2][0];
	const float SrcB21 = Right[2][1];
	const float SrcB22 = Right[2][2];

	Matrix3 Result;
	Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
	Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
	Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
	Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
	Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
	Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
	Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
	Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
	Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
	return Result;
}

inline Matrix3 operator/(float Left, const Matrix3& Right) noexcept
{
	return { Left / Right[0], Left / Right[1], Left / Right[2] };
}

inline Matrix3 operator/(const Matrix3& Left, float Right) noexcept
{
	return { Left[0] / Right, Left[1] / Right, Left[2] / Right };
}

inline Matrix3 operator/(const Matrix3& Left, const Matrix3& Right) noexcept
{
	Matrix3 m1_copy(Left);
	return m1_copy /= Right;
}

inline Matrix3& operator-=(Matrix3& Left, float Right) noexcept { return Left = Left - Right; }
inline Matrix3& operator-=(Matrix3& Left, const Matrix3& Right) noexcept { return Left = Left - Right; }
inline Matrix3& operator+=(Matrix3& Left, float Right) noexcept { return Left = Left + Right; }
inline Matrix3& operator+=(Matrix3& Left, const Matrix3& Right) noexcept { return Left = Left + Right; }
inline Matrix3& operator*=(Matrix3& Left, float Right) noexcept { return Left = Left * Right; }
inline Matrix3& operator*=(Matrix3& Left, const Matrix3& Right) noexcept { return Left = Left * Right; }
inline Matrix3& operator/=(Matrix3& Left, float Right) noexcept { return Left = Left / Right; }
inline Matrix3& operator/=(Matrix3& Left, const Matrix3& Right) noexcept { return Left = Left * Right.Inverse(); }

//=============================================================================
// Matrix4 Impl
//=============================================================================

const inline Matrix4 Matrix4::Zero = {
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
};

const inline Matrix4 Matrix4::Identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

inline constexpr Matrix4::Matrix4(
	float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2,
	float x3, float y3, float z3, float w3) 
	: value{ {x0, y0, z0, w0}, {x1, y1, z1, w1}, {x2, y2, z2, w2}, {x3, y3, z3, w3} }
{
}

inline constexpr Matrix4::Matrix4(const Matrix3& m) : value{ {m[0], 0.0f}, {m[1], 0.0f}, {m[2], 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} }
{
}

inline float Matrix4::GetDeterminant() const
{
	const float SubFactor00 = value[2][2] * value[3][3] - value[3][2] * value[2][3];
	const float SubFactor01 = value[2][1] * value[3][3] - value[3][1] * value[2][3];
	const float SubFactor02 = value[2][1] * value[3][2] - value[3][1] * value[2][2];
	const float SubFactor03 = value[2][0] * value[3][3] - value[3][0] * value[2][3];
	const float SubFactor04 = value[2][0] * value[3][2] - value[3][0] * value[2][2];
	const float SubFactor05 = value[2][0] * value[3][1] - value[3][0] * value[2][1];

	const Vector4 DetCof(
		+(value[1][1] * SubFactor00 - value[1][2] * SubFactor01 + value[1][3] * SubFactor02),
		-(value[1][0] * SubFactor00 - value[1][2] * SubFactor03 + value[1][3] * SubFactor04),
		+(value[1][0] * SubFactor01 - value[1][1] * SubFactor03 + value[1][3] * SubFactor05),
		-(value[1][0] * SubFactor02 - value[1][1] * SubFactor04 + value[1][2] * SubFactor05));

	return
		value[0][0] * DetCof[0] + value[0][1] * DetCof[1] +
		value[0][2] * DetCof[2] + value[0][3] * DetCof[3];

}

inline Matrix4 Matrix4::Inverse() const
{
	const float Coef00 = value[2][2] * value[3][3] - value[3][2] * value[2][3];
	const float Coef02 = value[1][2] * value[3][3] - value[3][2] * value[1][3];
	const float Coef03 = value[1][2] * value[2][3] - value[2][2] * value[1][3];

	const float Coef04 = value[2][1] * value[3][3] - value[3][1] * value[2][3];
	const float Coef06 = value[1][1] * value[3][3] - value[3][1] * value[1][3];
	const float Coef07 = value[1][1] * value[2][3] - value[2][1] * value[1][3];

	const float Coef08 = value[2][1] * value[3][2] - value[3][1] * value[2][2];
	const float Coef10 = value[1][1] * value[3][2] - value[3][1] * value[1][2];
	const float Coef11 = value[1][1] * value[2][2] - value[2][1] * value[1][2];

	const float Coef12 = value[2][0] * value[3][3] - value[3][0] * value[2][3];
	const float Coef14 = value[1][0] * value[3][3] - value[3][0] * value[1][3];
	const float Coef15 = value[1][0] * value[2][3] - value[2][0] * value[1][3];

	const float Coef16 = value[2][0] * value[3][2] - value[3][0] * value[2][2];
	const float Coef18 = value[1][0] * value[3][2] - value[3][0] * value[1][2];
	const float Coef19 = value[1][0] * value[2][2] - value[2][0] * value[1][2];

	const float Coef20 = value[2][0] * value[3][1] - value[3][0] * value[2][1];
	const float Coef22 = value[1][0] * value[3][1] - value[3][0] * value[1][1];
	const float Coef23 = value[1][0] * value[2][1] - value[2][0] * value[1][1];

	const Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
	const Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
	const Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
	const Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
	const Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
	const Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

	const Vector4 Vec0(value[1][0], value[0][0], value[0][0], value[0][0]);
	const Vector4 Vec1(value[1][1], value[0][1], value[0][1], value[0][1]);
	const Vector4 Vec2(value[1][2], value[0][2], value[0][2], value[0][2]);
	const Vector4 Vec3(value[1][3], value[0][3], value[0][3], value[0][3]);

	const Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	const Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	const Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	const Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	const Vector4 SignA(+1.0f, -1.0f, +1.0f, -1.0f);
	const Vector4 SignB(-1.0f, +1.0f, -1.0f, +1.0f);

	const Matrix4 inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	const Vector4 Row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

	const Vector4 Dot0(value[0] * Row0);
	const float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	const float OneOverDeterminant = 1.0f / Dot1;

	return inverse * OneOverDeterminant;
}

inline Matrix4 Matrix4::Transpose() const
{
	Matrix4 Result;
	Result[0][0] = value[0][0];
	Result[0][1] = value[1][0];
	Result[0][2] = value[2][0];
	Result[0][3] = value[3][0];

	Result[1][0] = value[0][1];
	Result[1][1] = value[1][1];
	Result[1][2] = value[2][1];
	Result[1][3] = value[3][1];

	Result[2][0] = value[0][2];
	Result[2][1] = value[1][2];
	Result[2][2] = value[2][2];
	Result[2][3] = value[3][2];

	Result[3][0] = value[0][3];
	Result[3][1] = value[1][3];
	Result[3][2] = value[2][3];
	Result[3][3] = value[3][3];
	return Result;
}

inline Matrix4 Matrix4::Translate(const Matrix4 & m, const Vector3 & v)
{
	Matrix4 Result(m);
	Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	return Result;
}

inline Matrix4 Matrix4::Rotate(const Matrix4& m, float angle, const Vector3& v)
{
	const float c = cos(angle);
	const float s = sin(angle);

	Vector3 axis(v.GetNormalize());
	Vector3 temp((1.0f - c) * axis);

	Matrix4 Rotate;
	Rotate[0][0] = c + temp[0] * axis[0];
	Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
	Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

	Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
	Rotate[1][1] = c + temp[1] * axis[1];
	Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

	Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
	Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
	Rotate[2][2] = c + temp[2] * axis[2];

	Matrix4 Result;
	Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
	Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
	Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
	Result[3] = m[3];
	return Result;
}

inline Matrix4 Matrix4::Scale(const Matrix4& m, const Vector3& v)
{
	Matrix4 Result;
	Result[0] = m[0] * v[0];
	Result[1] = m[1] * v[1];
	Result[2] = m[2] * v[2];
	Result[3] = m[3];
	return Result;
}

inline Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 Result = Matrix4::Identity;
	Result[0][0] = 2.0f / (right - left);
	Result[1][1] = 2.0f / (top - bottom);
	Result[2][2] = 2.0f / (zFar - zNear);
	Result[3][0] = -(right + left) / (right - left);
	Result[3][1] = -(top + bottom) / (top - bottom);
	Result[3][2] = -(zFar + zNear) / (zFar - zNear);
	return Result;
}

inline Matrix4 Matrix4::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 Result = Matrix4::Zero;
	Result[0][0] = (2.0f * zNear) / (right - left);
	Result[1][1] = (2.0f * zNear) / (top - bottom);
	Result[2][0] = -(right + left) / (right - left);
	Result[2][1] = -(top + bottom) / (top - bottom);
	Result[2][2] = (zFar + zNear) / (zFar - zNear);
	Result[2][3] = 1.0f;
	Result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
	return Result;
}

inline Matrix4 Matrix4::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

	const float tanHalfFovy = tan(fovy / 2.0f);

	Matrix4 Result = Matrix4::Zero;
	Result[0][0] = 1.0f / (aspect * tanHalfFovy);
	Result[1][1] = 1.0f / (tanHalfFovy);
	Result[2][2] = (zFar + zNear) / (zFar - zNear);
	Result[2][3] = 1.0f;
	Result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
	return Result;
}

inline Matrix4 Matrix4::PerspectiveFov(float fov, float width, float height, float zNear, float zFar)
{
	assert(width > 0.0f);
	assert(height > 0.0f);
	assert(fov > 0.0f);

	const float rad = fov;
	const float h = cos(0.5f * rad) / sin(0.5f * rad);
	const float w = h * height / width;

	Matrix4 Result = Matrix4::Zero;
	Result[0][0] = w;
	Result[1][1] = h;
	Result[2][2] = (zFar + zNear) / (zFar - zNear);
	Result[2][3] = 1.0f;
	Result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
	return Result;
}

inline Matrix4 Matrix4::InfinitePerspective(float fovy, float aspect, float zNear)
{
	const float range = tan(fovy / 2.0f) * zNear;
	const float left = -range * aspect;
	const float right = range * aspect;
	const float bottom = -range;
	const float top = range;

	Matrix4 Result = Matrix4::Zero;
	Result[0][0] = (2.0f * zNear) / (right - left);
	Result[1][1] = (2.0f * zNear) / (top - bottom);
	Result[2][2] = 1.0f;
	Result[2][3] = 1.0f;
	Result[3][2] = -2.0f * zNear;
	return Result;
}

inline Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	const Vector3 f((center - eye).GetNormalize());
	const Vector3 s(CrossProduct(up, f).GetNormalize());
	const Vector3 u(CrossProduct(f, s));

	Matrix4 Result = Matrix4::Identity;
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = f.x;
	Result[1][2] = f.y;
	Result[2][2] = f.z;
	Result[3][0] = -DotProduct(s, eye);
	Result[3][1] = -DotProduct(u, eye);
	Result[3][2] = -DotProduct(f, eye);
	return Result;
}

inline Vector3 Matrix4::Project(const Vector3& obj, const Matrix4& model, const Matrix4& proj, const Vector4& viewport)
{
	Vector4 tmp = Vector4(obj, 1.0f);
	tmp = model * tmp;
	tmp = proj * tmp;

	tmp /= tmp.w;
	tmp = tmp * 0.5f + 0.5f;
	tmp[0] = tmp[0] * (viewport[2]) + (viewport[0]);
	tmp[1] = tmp[1] * (viewport[3]) + (viewport[1]);

	return Vector3(tmp);
}

inline Vector3 Matrix4::UnProject(const Vector3& win, const Matrix4& model, const Matrix4& proj, const Vector4& viewport)
{
	Matrix4 inverse = (proj * model).Inverse();

	Vector4 tmp = Vector4(win, 1.0f);
	tmp.x = (tmp.x - (viewport[0])) / (viewport[2]);
	tmp.y = (tmp.y - (viewport[1])) / (viewport[3]);
	tmp = tmp * 2.0f - 1.0f;

	Vector4 obj = inverse * tmp;
	obj /= obj.w;

	return Vector3(obj);
}

inline Matrix4 Matrix4::PickMatrix(const Vector2& center, const Vector2& delta, const Vector4& viewport)
{
	assert(delta.x > 0.0f && delta.y > 0.0f);
	Matrix4 Result = Matrix4::Identity;

	if( !(delta.x > 0.0f && delta.y > 0.0f) )
		return Result; // Error

	Vector3 Temp(
		((viewport[2]) - 2.0f * (center.x - (viewport[0]))) / delta.x,
		((viewport[3]) - 2.0f * (center.y - (viewport[1]))) / delta.y,
		0.0f);

	// Translate and scale the picked region to the entire window
	Result = Matrix4::Translate(Result, Temp);
	return Matrix4::Scale(Result, Vector3((viewport[2]) / delta.x, (viewport[3]) / delta.y, 1.0f));
}

inline Matrix4 CastToMatrix4(const Quaternion& q)
{
	return Matrix4(Matrix3(q));
}

inline bool operator==(const Matrix4& Left, const Matrix4& Right) noexcept
{
	return ( Left[0] == Right[0] && Left[1] == Right[1] && Left[2] == Right[2] && Left[3] == Right[3] );
}

inline bool operator!=(const Matrix4& Left, const Matrix4& Right) noexcept
{
	return !(Left == Right);
}

inline Matrix4 operator-(const Matrix4& m) noexcept
{
	return { -m[0], -m[1], -m[2], -m[3] };
}

inline Matrix4 operator-(float Left, const Matrix4& Right) noexcept
{
	return { Left - Right[0], Left - Right[1], Left - Right[2], Left - Right[3] };
}

inline Matrix4 operator-(const Matrix4& Left, float Right) noexcept
{
	return { Left[0] - Right, Left[1] - Right, Left[2] - Right, Left[3] - Right };
}

inline Matrix4 operator-(const Matrix4& Left, const Matrix4& Right) noexcept
{
	return { Left[0] - Right[0], Left[1] - Right[1], Left[2] - Right[2], Left[3] - Right[3] };
}

inline Matrix4 operator+(float Left, const Matrix4& Right) noexcept
{
	return Right + Left;
}

inline Matrix4 operator+(const Matrix4& Left, float Right) noexcept
{
	return { Left[0] + Right, Left[1] + Right, Left[2] + Right, Left[3] + Right };
}

inline Matrix4 operator+(const Matrix4& Left, const Matrix4& Right) noexcept
{
	return { Left[0] + Right[0], Left[1] + Right[1], Left[2] + Right[2], Left[3] + Right[3] };
}

inline Matrix4 operator*(float Left, const Matrix4& Right) noexcept
{
	return Right * Left;
}

inline Matrix4 operator*(const Matrix4& Left, float Right) noexcept
{
	return { Left[0] * Right, Left[1] * Right, Left[2] * +Right, Left[3] * Right };
}

inline Matrix4 operator*(const Matrix4& Left, const Matrix4& Right) noexcept
{
	// TODO: убрать лишние операции
	const Vector4& SrcA0 = Left[0];
	const Vector4& SrcA1 = Left[1];
	const Vector4& SrcA2 = Left[2];
	const Vector4& SrcA3 = Left[3];
	const Vector4& SrcB0 = Right[0];
	const Vector4& SrcB1 = Right[1];
	const Vector4& SrcB2 = Right[2];
	const Vector4& SrcB3 = Right[3];

	Matrix4 Result;
	Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
	return Result;
}

inline Matrix4 operator/(float Left, const Matrix4& Right) noexcept
{
	return { Left / Right[0], Left / Right[1], Left / Right[2], Left / Right[3] };
}

inline Matrix4 operator/(const Matrix4& Left, float Right) noexcept
{
	return { Left[0] / Right, Left[1] / Right, Left[2] / Right, Left[3] / Right };
}

inline Matrix4 operator/(const Matrix4& Left, const Matrix4& Right) noexcept
{
	Matrix4 m1_copy(Left);
	return m1_copy /= Right;
}

inline Matrix4& operator-=(Matrix4& Left, float Right) noexcept { return Left = Left - Right; }
inline Matrix4& operator-=(Matrix4& Left, const Matrix4& Right) noexcept { return Left = Left - Right; }
inline Matrix4& operator+=(Matrix4& Left, float Right) noexcept { return Left = Left + Right; }
inline Matrix4& operator+=(Matrix4& Left, const Matrix4& Right) noexcept { return Left = Left + Right; }
inline Matrix4& operator*=(Matrix4& Left, float Right) noexcept { return Left = Left * Right; }
inline Matrix4& operator*=(Matrix4& Left, const Matrix4& Right) noexcept { return Left = Left * Right; }
inline Matrix4& operator/=(Matrix4& Left, float Right) noexcept { return Left = Left / Right; }
inline Matrix4& operator/=(Matrix4& Left, const Matrix4& Right) noexcept { return Left = Left * Right.Inverse(); }


//=============================================================================
// Matrix3Old Impl
//=============================================================================

inline constexpr Matrix3_old::Matrix3_old(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	Set(x0, y0, z0, x1, y1, z1, x2, y2, z2);
}

inline Matrix3_old::Matrix3_old(const Matrix4_old& m)
{
	Set(m[0], m[1], m[2], 
		m[4], m[5], m[6], 
		m[8], m[9], m[10]);
}

inline constexpr void Matrix3_old::Set(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	m[0] = x0; m[1] = y0; m[2] = z0;
	m[3] = x1; m[4] = y1; m[5] = z1;
	m[6] = x2; m[7] = y2; m[8] = z2;
}

inline constexpr void Matrix3_old::Set(const Matrix3_old& M)
{
	*this = M;
}

inline float Matrix3_old::GetDeterminant() const
{
	return
		+ m[0] * (m[4] * m[8] - m[7] * m[5])
		- m[3] * (m[1] * m[8] - m[7] * m[2])
		+ m[6] * (m[1] * m[5] - m[4] * m[2]);
}

inline Matrix3_old Matrix3_old::Transpose() const
{
	Matrix3_old transposed;
	transposed[0] = m[0];
	transposed[1] = m[3];
	transposed[2] = m[6];
	transposed[3] = m[1];
	transposed[4] = m[4];
	transposed[5] = m[7];
	transposed[6] = m[2];
	transposed[7] = m[5];
	transposed[8] = m[8];
	return transposed;
}

inline Matrix3_old Matrix3_old::Inverse() const
{
	const float OneOverDeterminant = 1.0f / (
		+ m[0] * (m[4] * m[8] - m[7] * m[5])
		- m[3] * (m[1] * m[8] - m[7] * m[2])
		+ m[6] * (m[1] * m[5] - m[4] * m[2]));

	Matrix3_old Inverse;
	Inverse[0] = +(m[4] * m[8] - m[7] * m[5]) * OneOverDeterminant;
	Inverse[3] = -(m[3] * m[8] - m[6] * m[5]) * OneOverDeterminant;
	Inverse[6] = +(m[3] * m[7] - m[6] * m[4]) * OneOverDeterminant;
	Inverse[1] = -(m[1] * m[8] - m[7] * m[2]) * OneOverDeterminant;
	Inverse[4] = +(m[0] * m[8] - m[6] * m[2]) * OneOverDeterminant;
	Inverse[7] = -(m[0] * m[7] - m[6] * m[1]) * OneOverDeterminant;
	Inverse[2] = +(m[1] * m[5] - m[4] * m[2]) * OneOverDeterminant;
	Inverse[5] = -(m[0] * m[5] - m[3] * m[2]) * OneOverDeterminant;
	Inverse[8] = +(m[0] * m[4] - m[3] * m[1]) * OneOverDeterminant;
	return Inverse;
}

inline void Matrix3_old::Scale(const Vector3& scale)
{
	m[0] *= scale.x;
	m[4] *= scale.y;
	m[8] *= scale.z;
}

inline Matrix3_old Matrix3Scale(const Vector3& scale)
{
	Matrix3_old ret;
	ret[0] = scale.x; ret[3] = 0.0f;    ret[6] = 0.0f;
	ret[1] = 0.0f;    ret[4] = scale.y; ret[7] = 0.0f;
	ret[2] = 0.0f;    ret[5] = 0.0f;    ret[8] = scale.z;
	return ret;
}

inline Matrix3_old Matrix3RotationX(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix3_old ret;
	ret[0] = 1.0f; ret[3] = 0.0f; ret[6] = 0.0f;
	ret[1] = 0.0f; ret[4] = c;    ret[7] = -s;
	ret[2] = 0.0f; ret[5] = s;    ret[8] = c;
	return ret;
}

inline Matrix3_old Matrix3RotationY(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix3_old ret;
	ret[0] = c;    ret[3] = 0.0f; ret[6] = s;
	ret[1] = 0.0f; ret[4] = 1.0f; ret[7] = 0.0f;
	ret[2] = -s;   ret[5] = 0.0f; ret[8] = c;
	return ret;
}

inline Matrix3_old Matrix3RotationZ(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix3_old ret;
	ret[0] = c;    ret[3] = -s;   ret[6] = 0.0f;
	ret[1] = s;    ret[4] = c;    ret[7] = 0.0f;
	ret[2] = 0.0f; ret[5] = 0.0f; ret[8] = 1.0f;
	return ret;
}

inline Matrix3_old Matrix3Rotation(const Quaternion& q)
{
	const float xx = q[0] * q[0];
	const float yy = q[1] * q[1];
	const float zz = q[2] * q[2];
	const float xy = q[0] * q[1];
	const float zw = q[2] * q[3];
	const float xz = q[8] * q[0];
	const float yw = q[1] * q[3];
	const float yz = q[1] * q[2];
	const float xw = q[0] * q[3];
	Matrix3_old ret;
	ret[0] = 1.0f - 2.0f * (yy - zz);
	ret[1] = 2.0f * (xy + zw);
	ret[2] = 2.0f * (xz - yw);
	ret[3] = 2.0f * (xy - zw);
	ret[4] = 1.0f - 2.0f * (xx - zz);
	ret[5] = 2.0f * (yz + xw);
	ret[6] = 2.0f * (xz + yw);
	ret[7] = 2.0f * (yz - xw);
	ret[8] = 1.0f - 2.0f * (xx - yy);
	return ret;
}

inline bool operator==(const Matrix3_old& Left, const Matrix3_old& Right) noexcept
{
	return (
		Left[0] == Right[0] &&
		Left[1] == Right[1] &&
		Left[2] == Right[2] &&
		Left[3] == Right[3] &&
		Left[4] == Right[4] &&
		Left[5] == Right[5] &&
		Left[6] == Right[6] &&
		Left[7] == Right[7] &&
		Left[8] == Right[8]);
}

inline bool operator!=(const Matrix3_old& Left, const Matrix3_old& Right) noexcept
{
	return !(Left == Right);
}

inline Matrix3_old operator-(const Matrix3_old& In) noexcept
{
	return {
		-In[0], -In[1], -In[2],
		-In[3], -In[4], -In[5],
		-In[6], -In[7], -In[8]
	};
}

inline Matrix3_old operator-(float Left, const Matrix3_old& Right) noexcept
{
	return {
		Left - Right[0], Left - Right[1], Left - Right[2],
		Left - Right[3], Left - Right[4], Left - Right[5],
		Left - Right[6], Left - Right[7], Left - Right[8]
	};
}

inline Matrix3_old operator-(const Matrix3_old& Left, float Right) noexcept
{
	return {
		Left[0] - Right, Left[1] - Right, Left[2] - Right,
		Left[3] - Right, Left[4] - Right, Left[5] - Right,
		Left[6] - Right, Left[7] - Right, Left[8] - Right
	};
}

inline Matrix3_old operator-(const Matrix3_old& Left, const Matrix3_old& Right) noexcept
{
	return {
		Left[0] - Right[0], Left[1] - Right[1], Left[2] - Right[2],
		Left[3] - Right[3], Left[4] - Right[4], Left[5] - Right[5],
		Left[6] - Right[6], Left[7] - Right[7], Left[8] - Right[8]
	};
}

inline Matrix3_old operator+(float Left, const Matrix3_old& Right) noexcept
{
	return Right + Left;
}

inline Matrix3_old operator+(const Matrix3_old& Left, float Right) noexcept
{
	return {
		Left[0] + Right, Left[1] + Right, Left[2] + Right,
		Left[3] + Right, Left[4] + Right, Left[5] + Right,
		Left[6] + Right, Left[7] + Right, Left[8] + Right
	};
}

inline Matrix3_old operator+(const Matrix3_old& Left, const Matrix3_old& Right) noexcept
{
	return {
		Left[0] + Right[0], Left[1] + Right[1], Left[2] + Right[2],
		Left[3] + Right[3], Left[4] + Right[4], Left[5] + Right[5],
		Left[6] + Right[6], Left[7] + Right[7], Left[8] + Right[8]
	};
}

inline Matrix3_old operator*(float Left, const Matrix3_old& Right) noexcept
{
	return Right * Left;
}

inline Matrix3_old operator*(const Matrix3_old& Left, float Right) noexcept
{
	return {
		Left[0] * Right, Left[1] * Right, Left[2] * Right,
		Left[3] * Right, Left[4] * Right, Left[5] * Right,
		Left[6] * Right, Left[7] * Right, Left[8] * Right
	};
}

inline Matrix3_old operator*(const Matrix3_old& Left, const Matrix3_old& Right) noexcept
{
	return {
		Left[0] * Right[0] + Left[3] * Right[1] + Left[6] * Right[2],
		Left[1] * Right[0] + Left[4] * Right[1] + Left[7] * Right[2],
		Left[2] * Right[0] + Left[5] * Right[1] + Left[8] * Right[2],
		Left[0] * Right[3] + Left[3] * Right[4] + Left[6] * Right[5],
		Left[1] * Right[3] + Left[4] * Right[4] + Left[7] * Right[5],
		Left[2] * Right[3] + Left[5] * Right[4] + Left[8] * Right[5],
		Left[0] * Right[6] + Left[3] * Right[7] + Left[6] * Right[8],
		Left[1] * Right[6] + Left[4] * Right[7] + Left[7] * Right[8],
		Left[2] * Right[6] + Left[5] * Right[7] + Left[8] * Right[8]
	};
}

inline Matrix3_old operator/(float Left, const Matrix3_old& Right) noexcept
{
	return {
		Left / Right[0], Left / Right[1], Left / Right[2],
		Left / Right[3], Left / Right[4], Left / Right[5],
		Left / Right[6], Left / Right[7], Left / Right[8]
	};
}

inline Matrix3_old operator/(const Matrix3_old& Left, float Right) noexcept
{
	return {
		Left[0] / Right, Left[1] / Right, Left[2] / Right,
		Left[3] / Right, Left[4] / Right, Left[5] / Right,
		Left[6] / Right, Left[7] / Right, Left[8] / Right
	};
}

inline Matrix3_old operator/(const Matrix3_old& Left, const Matrix3_old& Right) noexcept
{
	Matrix3_old m1_copy(Left);
	return m1_copy /= Right;
}

inline Matrix3_old& operator-=(Matrix3_old& Left, float Right) noexcept { return Left = Left - Right; }
inline Matrix3_old& operator-=(Matrix3_old& Left, const Matrix3_old& Right) noexcept { return Left = Left - Right; }
inline Matrix3_old& operator+=(Matrix3_old& Left, float Right) noexcept { return Left = Left + Right; }
inline Matrix3_old& operator+=(Matrix3_old& Left, const Matrix3_old& Right) noexcept { return Left = Left + Right; }
inline Matrix3_old& operator*=(Matrix3_old& Left, float Right) noexcept { return Left = Left * Right; }
inline Matrix3_old& operator*=(Matrix3_old& Left, const Matrix3_old& Right) noexcept { return Left = Left * Right; }
inline Matrix3_old& operator/=(Matrix3_old& Left, float Right) noexcept { return Left = Left / Right; }
inline Matrix3_old& operator/=(Matrix3_old& Left, const Matrix3_old& Right) noexcept { return Left = Left * Right.Inverse(); }


//=============================================================================
// Matrix4Old Impl
//=============================================================================

inline constexpr Matrix4_old::Matrix4_old(
	float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2,
	float x3, float y3, float z3, float w3)
{
	Set(x0, y0, z0, w0, 
		x1, y1, z1, w1, 
		x2, y2, z2, w2, 
		x3, y3, z3, w3);
}

inline constexpr Matrix4_old::Matrix4_old(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
{
	Set(v0.x, v0.y, v0.z, v0.w,
		v1.x, v1.y, v1.z, v1.w,
		v2.x, v2.y, v2.z, v2.w,
		v3.x, v3.y, v3.z, v3.w);
}

inline constexpr Matrix4_old::Matrix4_old(const Matrix3_old& m)
{
	Set(m[0], m[1], m[2], 0.0f,
		m[3], m[4], m[5], 0.0f,
		m[6], m[7], m[8], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline constexpr void Matrix4_old::Set(
	float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2,
	float x3, float y3, float z3, float w3)
{
	m[ 0] = x0; m[ 1] = y0; m[ 2] = z0; m[ 3] = w0;
	m[ 4] = x1; m[ 5] = y1; m[ 6] = z1; m[ 7] = w1;
	m[ 8] = x2; m[ 9] = y2; m[10] = z2; m[11] = w2;
	m[12] = x3; m[13] = y3; m[14] = z3; m[15] = w3;
}

inline constexpr void Matrix4_old::Set(const Matrix4_old& M)
{
	*this = M;
}

inline void Matrix4_old::Scale(const Vector3& scale)
{
	m[0] *= scale.x;
	m[5] *= scale.y;
	m[10] *= scale.z;
}

inline void Matrix4_old::Translate(const Vector3& pos)
{
	m[12] += pos.x;
	m[13] += pos.y;
	m[14] += pos.z;
}

inline float Matrix4_old::GetDeterminant() const
{
	const float SubFactor00 = m[10] * m[15] - m[14] * m[11];
	const float SubFactor01 = m[ 9] * m[15] - m[13] * m[11];
	const float SubFactor02 = m[ 9] * m[14] - m[13] * m[10];
	const float SubFactor03 = m[ 8] * m[15] - m[12] * m[11];
	const float SubFactor04 = m[ 8] * m[14] - m[12] * m[10];
	const float SubFactor05 = m[ 8] * m[13] - m[12] * m[ 9];

	const Vector4 DetCof(
		+(m[5] * SubFactor00 - m[6] * SubFactor01 + m[7] * SubFactor02),
		-(m[4] * SubFactor00 - m[6] * SubFactor03 + m[7] * SubFactor04),
		+(m[4] * SubFactor01 - m[5] * SubFactor03 + m[7] * SubFactor05),
		-(m[4] * SubFactor02 - m[5] * SubFactor04 + m[6] * SubFactor05));

	return m[0] * DetCof[0] + m[1] * DetCof[1] + m[2] * DetCof[2] + m[3] * DetCof[3];
}

inline Matrix4_old Matrix4_old::Transpose() const
{
	Matrix4_old transposed;
	transposed[ 0] = m[ 0];
	transposed[ 1] = m[ 4];
	transposed[ 2] = m[ 8];
	transposed[ 3] = m[12];
	transposed[ 4] = m[ 1];
	transposed[ 5] = m[ 5];
	transposed[ 6] = m[ 9];
	transposed[ 7] = m[13];
	transposed[ 8] = m[ 2];
	transposed[ 9] = m[ 6];
	transposed[10] = m[10];
	transposed[11] = m[14];
	transposed[12] = m[ 3];
	transposed[13] = m[ 7];
	transposed[14] = m[11];
	transposed[15] = m[15];
	return transposed;
}

inline Matrix4_old Matrix4_old::Inverse() const
{
	const float Coef00 = m[10] * m[15] - m[14] * m[11];
	const float Coef02 = m[6] * m[15] - m[14] * m[7];
	const float Coef03 = m[6] * m[11] - m[10] * m[7];

	const float Coef04 = m[9] * m[15] - m[13] * m[11];
	const float Coef06 = m[5] * m[15] - m[13] * m[7];
	const float Coef07 = m[5] * m[11] - m[9] * m[7];

	const float Coef08 = m[9] * m[14] - m[13] * m[10];
	const float Coef10 = m[5] * m[14] - m[13] * m[6];
	const float Coef11 = m[5] * m[10] - m[9] * m[6];

	const float Coef12 = m[8] * m[15] - m[12] * m[11];
	const float Coef14 = m[4] * m[15] - m[12] * m[7];
	const float Coef15 = m[4] * m[11] - m[8] * m[7];

	const float Coef16 = m[8] * m[14] - m[12] * m[10];
	const float Coef18 = m[4] * m[14] - m[12] * m[6];
	const float Coef19 = m[4] * m[10] - m[8] * m[6];

	const float Coef20 = m[8] * m[13] - m[12] * m[9];
	const float Coef22 = m[4] * m[13] - m[12] * m[5];
	const float Coef23 = m[4] * m[9] - m[8] * m[5];

	const Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
	const Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
	const Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
	const Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
	const Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
	const Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

	const Vector4 Vec0(m[4], m[0], m[0], m[0]);
	const Vector4 Vec1(m[5], m[1], m[1], m[1]);
	const Vector4 Vec2(m[6], m[2], m[2], m[2]);
	const Vector4 Vec3(m[7], m[3], m[3], m[3]);

	const Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	const Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	const Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	const Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	const Vector4 SignA(+1, -1, +1, -1);
	const Vector4 SignB(-1, +1, -1, +1);
	const Matrix4_old Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	const Vector4 Row0(Inverse[0], Inverse[4], Inverse[8], Inverse[12]);

	const Vector4 Dot0(m[0] * Row0);
	const float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	const float OneOverDeterminant = 1.0f / Dot1;

	return Inverse * OneOverDeterminant;
}

inline Matrix4_old Cofactor(const Matrix3_old& m0)
{
	Matrix4_old cofactor;
	Matrix3_old minor;
	minor[0] = m0[5];
	minor[1] = m0[6];
	minor[2] = m0[7];
	minor[3] = m0[9];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[13];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[0] = minor.GetDeterminant();
	minor[0] = m0[4];
	minor[1] = m0[6];
	minor[2] = m0[7];
	minor[3] = m0[8];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[1] = -minor.GetDeterminant();
	minor[0] = m0[4];
	minor[1] = m0[5];
	minor[2] = m0[7];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[15];
	cofactor[2] = minor.GetDeterminant();
	minor[0] = m0[4];
	minor[1] = m0[5];
	minor[2] = m0[6];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[10];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[14];
	cofactor[3] = -minor.GetDeterminant();
	minor[0] = m0[1];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[9];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[13];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[4] = -minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[8];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[5] = minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[3];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[15];
	cofactor[6] = -minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[2];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[10];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[14];
	cofactor[7] = minor.GetDeterminant();
	minor[0] = m0[1];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[5];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[13];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[8] = minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[12];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[9] = -minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[7];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[15];
	cofactor[10] = minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[2];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[6];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[14];
	cofactor[11] = -minor.GetDeterminant();
	minor[0] = m0[1];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[5];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[9];
	minor[7] = m0[10];
	minor[8] = m0[11];
	cofactor[12] = -minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[8];
	minor[7] = m0[10];
	minor[8] = m0[11];
	cofactor[13] = minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[7];
	minor[6] = m0[8];
	minor[7] = m0[9];
	minor[8] = m0[11];
	cofactor[14] = -minor.GetDeterminant();
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[2];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[6];
	minor[6] = m0[8];
	minor[7] = m0[9];
	minor[8] = m0[10];
	cofactor[15] = minor.GetDeterminant();
	return cofactor;
}

inline Matrix4_old Matrix4Scale(const Vector3& scale)
{
	Matrix4_old mat;
	mat[0] = scale.x; mat[4] = 0.0f;    mat[ 8] = 0.0f;    mat[12] = 0.0f;
	mat[1] = 0.0f;    mat[5] = scale.y; mat[ 9] = 0.0f;    mat[13] = 0.0f;
	mat[2] = 0.0f;    mat[6] = 0.0f;    mat[10] = scale.z; mat[14] = 0.0f;
	mat[3] = 0.0f;    mat[7] = 0.0f;    mat[11] = 0.0f;    mat[15] = 1.0f;
	return mat;
}

inline Matrix4_old Matrix4RotationX(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix4_old ret;
	ret[0] = 1.0f; ret[4] = 0.0f; ret[ 8] = 0.0f; ret[12] = 0.0f;
	ret[1] = 0.0f; ret[5] = c;    ret[ 9] = -s;   ret[13] = 0.0f;
	ret[2] = 0.0f; ret[6] = s;    ret[10] = c;    ret[14] = 0.0f;
	ret[3] = 0.0f; ret[7] = 0.0f; ret[11] = 0.0f; ret[15] = 1.0f;
	return ret;
}

inline Matrix4_old Matrix4RotationY(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4_old mat;
	mat[0] = c;    mat[4] = 0.0f; mat[ 8] = s;    mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[ 9] = 0.0f; mat[13] = 0.0f;
	mat[2] = -s;   mat[6] = 0.0f; mat[10] = c;    mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4_old Matrix4RotationZ(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4_old mat;
	mat[0] = c;    mat[4] = -s;   mat[ 8] = 0.0f; mat[12] = 0.0f;
	mat[1] = s;    mat[5] = c;    mat[ 9] = 0.0f; mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4_old Matrix4Rotation(const Vector3& axis, float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);
	const float one_c = 1.0f - c;

	const float xx = axis.x * axis.x;
	const float xy = axis.x * axis.y;
	const float xz = axis.x * axis.z;
	const float yy = axis.y * axis.y;
	const float yz = axis.y * axis.z;
	const float zz = axis.z * axis.z;
	const float l = xx + yy + zz;
	const float sqrt_l = sqrtf(l);

	Matrix4_old mat;
	mat[0] = (xx + (yy + zz) * c) / l;
	mat[1] = (xy * one_c + axis.z * sqrt_l * s) / l;
	mat[2] = (xz * one_c - axis.y * sqrt_l * s) / l;
	mat[3] = 0.0f;
	mat[4] = (xy * one_c - axis.z * sqrt_l * s) / l;
	mat[5] = (yy + (xx + zz) * c) / l;
	mat[6] = (yz * one_c + axis.x * sqrt_l * s) / l;
	mat[7] = 0.0f;
	mat[8] = (xz * one_c + axis.y * sqrt_l * s) / l;
	mat[9] = (yz * one_c - axis.x * sqrt_l * s) / l;
	mat[10] = (zz + (xx + yy) * c) / l;
	mat[11] = 0.0f;
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
	return mat;
}

inline Matrix4_old Matrix4Rotation(const Quaternion& q0)
{
	const float xx = q0[0] * q0[0];
	const float yy = q0[1] * q0[1];
	const float zz = q0[2] * q0[2];
	const float xy = q0[0] * q0[1];
	const float zw = q0[2] * q0[3];
	const float xz = q0[0] * q0[2];
	const float yw = q0[1] * q0[3];
	const float yz = q0[1] * q0[2];
	const float xw = q0[0] * q0[3];
	Matrix4_old mat;
	mat[0] = 1.0f - 2.0f * (yy + zz);
	mat[1] = 2.0f * (xy + zw);
	mat[2] = 2.0f * (xz - yw);
	mat[3] = 0.0f;
	mat[4] = 2.0f * (xy - zw);
	mat[5] = 1.0f - 2.0f * (xx + zz);
	mat[6] = 2.0f * (yz + xw);
	mat[7] = 0.0f;
	mat[8] = 2.0f * (xz + yw);
	mat[9] = 2.0f * (yz - xw);
	mat[10] = 1.0f - 2.0f * (xx + yy);
	mat[11] = 0.0f;
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
	return mat;
}

inline Matrix4_old Matrix4Translate(const Vector3 & v)
{
	Matrix4_old mat;
	mat[0] = 1.0f; mat[4] = 0.0f; mat[ 8] = 0.0f; mat[12] = v.x;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[ 9] = 0.0f; mat[13] = v.y;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = v.z;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4_old LookAt(const Vector3& eye, const Vector3& dir, const Vector3& up)
{
	Vector3 forward = (dir - eye).GetNormalize();
	Vector3 side = CrossProduct(up, forward).GetNormalize();
	Vector3 lookUp = CrossProduct(forward, side);
	Matrix4_old m0;
	m0[0] = side.x;    m0[4] = side.y;    m0[ 8] = side.z;    m0[12] = -DotProduct(side, eye);
	m0[1] = lookUp.x;  m0[5] = lookUp.y;  m0[ 9] = lookUp.z;  m0[13] = -DotProduct(lookUp, eye); // TODO: проверить
	m0[2] = forward.x; m0[6] = forward.y; m0[10] = forward.z; m0[14] = -DotProduct(forward, eye);
	m0[3] = 0.0f;      m0[7] = 0.0f;      m0[11] = 0.0f;      m0[15] = 1.0f;
	return m0;
}

inline Matrix4_old Ortho(float left, float right, float bottom, float top, float n, float f)
{
	Matrix4_old result;
	result[0] = 2.0f / (right - left);
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 2.0f / (top - bottom);
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = -2.0f / (f - n);
	result[11] = 0.0f;
	result[12] = -((right + left) / (right - left));
	result[13] = -((top + bottom) / (top - bottom));
	result[14] = -((f + n) / (f - n));
	result[15] = 1.0f;
	return result;
}

inline Matrix4_old Perspective(float fov_y, float aspect, float zNear, float zFar)
{
	const float tanHalfFovY = tanf(fov_y * 0.5f);
	Matrix4_old mat;
	mat[ 0] = 1.0f / (aspect * tanHalfFovY);
	mat[ 1] = 0.0f;
	mat[ 2] = 0.0f;
	mat[ 3] = 0.0f;
	mat[ 4] = 0.0f;
	mat[ 5] = 1.0f / tanHalfFovY;
	mat[ 6] = 0.0f;
	mat[ 7] = 0.0f;
	mat[ 8] = 0.0f;
	mat[ 9] = 0.0f;
	mat[10] = (zFar + zNear) / (zFar - zNear);
	mat[11] = 1.0f;
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = -(2.0f * zFar * zNear) / (zFar - zNear);
	mat[15] = 0.0f;
	return mat;
}

inline bool operator==(const Matrix4_old& Left, const Matrix4_old& Right) noexcept
{
	return (
		Left[0] == Right[0] &&
		Left[1] == Right[1] &&
		Left[2] == Right[2] &&
		Left[3] == Right[3] &&
		Left[4] == Right[4] &&
		Left[5] == Right[5] &&
		Left[6] == Right[6] &&
		Left[7] == Right[7] &&
		Left[8] == Right[8] &&
		Left[9] == Right[9] &&
		Left[10] == Right[10] &&
		Left[11] == Right[11] &&
		Left[12] == Right[12] &&
		Left[13] == Right[13] &&
		Left[14] == Right[14] &&
		Left[15] == Right[15]);
}

inline bool operator!=(const Matrix4_old& Left, const Matrix4_old& Right) noexcept
{
	return !(Left == Right);
}

inline Matrix4_old operator-(const Matrix4_old& In) noexcept
{
	return {
		-In[ 0], -In[ 1], -In[ 2], -In[ 3],
		-In[ 4], -In[ 5], -In[ 6], -In[ 7],
		-In[ 8], -In[ 9], -In[10], -In[11],
		-In[12], -In[13], -In[14], -In[15]
	};
}

inline Matrix4_old operator-(float Left, const Matrix4_old& Right) noexcept
{
	return {
		Left - Right[ 0], Left - Right[ 1], Left - Right[ 2], Left - Right[ 3],
		Left - Right[ 4], Left - Right[ 5], Left - Right[ 6], Left - Right[ 7],
		Left - Right[ 8], Left - Right[ 9], Left - Right[10], Left - Right[11],
		Left - Right[12], Left - Right[13], Left - Right[14], Left - Right[15]
	};
}

inline Matrix4_old operator-(const Matrix4_old& Left, float Right) noexcept
{
	return {
		Left[ 0] - Right, Left[ 1] - Right, Left[ 2] - Right, Left[ 3] - Right,
		Left[ 4] - Right, Left[ 5] - Right, Left[ 6] - Right, Left[ 7] - Right,
		Left[ 8] - Right, Left[ 9] - Right, Left[10] - Right, Left[11] - Right,
		Left[12] - Right, Left[13] - Right, Left[14] - Right, Left[15] - Right
	};
}

inline Matrix4_old operator-(const Matrix4_old& Left, const Matrix4_old& Right) noexcept
{
	return {
		Left[ 0] - Right[ 0], Left[ 1] - Right[ 1], Left[ 2] - Right[ 2], Left[ 3] - Right[ 3],
		Left[ 4] - Right[ 4], Left[ 5] - Right[ 5], Left[ 6] - Right[ 6], Left[ 7] - Right[ 7],
		Left[ 8] - Right[ 8], Left[ 9] - Right[ 9], Left[10] - Right[10], Left[11] - Right[11],
		Left[12] - Right[12], Left[13] - Right[13], Left[14] - Right[14], Left[15] - Right[15]
	};
}

inline Matrix4_old operator+(float Left, const Matrix4_old& Right) noexcept
{
	return Right + Left;
}

inline Matrix4_old operator+(const Matrix4_old& Left, float Right) noexcept
{
	return {
		Left[ 0] + Right, Left[ 1] + Right, Left[ 2] + Right, Left[ 3] + Right, 
		Left[ 4] + Right, Left[ 5] + Right, Left[ 6] + Right, Left[ 7] + Right, 
		Left[ 8] + Right, Left[ 9] + Right, Left[10] + Right, Left[11] + Right,
		Left[12] + Right, Left[13] + Right, Left[14] + Right, Left[15] + Right
	};
}

inline Matrix4_old operator+(const Matrix4_old& Left, const Matrix4_old& Right) noexcept
{
	return {
		Left[ 0] + Right[ 0], Left[ 1] + Right[ 1], Left[ 2] + Right[ 2], Left[ 3] + Right[ 3],
		Left[ 4] + Right[ 4], Left[ 5] + Right[ 5], Left[ 6] + Right[ 6], Left[ 7] + Right[ 7],
		Left[ 8] + Right[ 8], Left[ 9] + Right[ 9], Left[10] + Right[10], Left[11] + Right[11],
		Left[12] + Right[12], Left[13] + Right[13], Left[14] + Right[14], Left[15] + Right[15]
	};
}

inline Matrix4_old operator*(float Left, const Matrix4_old& Right) noexcept
{
	return Right * Left;
}

inline Matrix4_old operator*(const Matrix4_old& Left, float Right) noexcept
{
	return {
		Left[ 0] * Right, Left[ 1] * Right, Left[ 2] *+ Right, Left[ 3] * Right,
		Left[ 4] * Right, Left[ 5] * Right, Left[ 6] *+ Right, Left[ 7] * Right,
		Left[ 8] * Right, Left[ 9] * Right, Left[10] *+ Right, Left[11] * Right,
		Left[12] * Right, Left[13] * Right, Left[14] *+ Right, Left[15] * Right
	};
}

inline Matrix4_old operator*(const Matrix4_old& Left, const Matrix4_old& Right) noexcept
{
	return {
		Left[0] * Right[ 0] + Left[4] * Right[ 1] + Left[ 8] * Right[ 2] + Left[12] * Right[ 3],
		Left[1] * Right[ 0] + Left[5] * Right[ 1] + Left[ 9] * Right[ 2] + Left[13] * Right[ 3],
		Left[2] * Right[ 0] + Left[6] * Right[ 1] + Left[10] * Right[ 2] + Left[14] * Right[ 3],
		Left[3] * Right[ 0] + Left[7] * Right[ 1] + Left[11] * Right[ 2] + Left[15] * Right[ 3],
		Left[0] * Right[ 4] + Left[4] * Right[ 5] + Left[ 8] * Right[ 6] + Left[12] * Right[ 7],
		Left[1] * Right[ 4] + Left[5] * Right[ 5] + Left[ 9] * Right[ 6] + Left[13] * Right[ 7],
		Left[2] * Right[ 4] + Left[6] * Right[ 5] + Left[10] * Right[ 6] + Left[14] * Right[ 7],
		Left[3] * Right[ 4] + Left[7] * Right[ 5] + Left[11] * Right[ 6] + Left[15] * Right[ 7],
		Left[0] * Right[ 8] + Left[4] * Right[ 9] + Left[ 8] * Right[10] + Left[12] * Right[11],
		Left[1] * Right[ 8] + Left[5] * Right[ 9] + Left[ 9] * Right[10] + Left[13] * Right[11],
		Left[2] * Right[ 8] + Left[6] * Right[ 9] + Left[10] * Right[10] + Left[14] * Right[11],
		Left[3] * Right[ 8] + Left[7] * Right[ 9] + Left[11] * Right[10] + Left[15] * Right[11],
		Left[0] * Right[12] + Left[4] * Right[13] + Left[ 8] * Right[14] + Left[12] * Right[15],
		Left[1] * Right[12] + Left[5] * Right[13] + Left[ 9] * Right[14] + Left[13] * Right[15],
		Left[2] * Right[12] + Left[6] * Right[13] + Left[10] * Right[14] + Left[14] * Right[15],
		Left[3] * Right[12] + Left[7] * Right[13] + Left[11] * Right[14] + Left[15] * Right[15]
	};
}

inline Matrix4_old operator/(float Left, const Matrix4_old& Right) noexcept
{
	return {
		Left / Right[ 0], Left / Right[ 1], Left / Right[ 2], Left / Right[ 3],
		Left / Right[ 4], Left / Right[ 5], Left / Right[ 6], Left / Right[ 7],
		Left / Right[ 8], Left / Right[ 9], Left / Right[10], Left / Right[11],
		Left / Right[12], Left / Right[13], Left / Right[14], Left / Right[15]
	};
}

inline Matrix4_old operator/(const Matrix4_old& Left, float Right) noexcept
{
	return {
		Left[ 0] / Right, Left[ 1] / Right, Left[ 2] / Right, Left[ 3] / Right,
		Left[ 4] / Right, Left[ 5] / Right, Left[ 6] / Right, Left[ 7] / Right,
		Left[ 8] / Right, Left[ 9] / Right, Left[10] / Right, Left[11] / Right,
		Left[12] / Right, Left[13] / Right, Left[14] / Right, Left[15] / Right
	};
}

inline Matrix4_old operator/(const Matrix4_old& Left, const Matrix4_old& Right) noexcept
{
	Matrix4_old m1_copy(Left);
	return m1_copy /= Right;
}

inline Matrix4_old& operator-=(Matrix4_old& Left, float Right) noexcept { return Left = Left - Right; }
inline Matrix4_old& operator-=(Matrix4_old& Left, const Matrix4_old& Right) noexcept { return Left = Left - Right; }
inline Matrix4_old& operator+=(Matrix4_old& Left, float Right) noexcept { return Left = Left + Right; }
inline Matrix4_old& operator+=(Matrix4_old& Left, const Matrix4_old& Right) noexcept { return Left = Left + Right; }
inline Matrix4_old& operator*=(Matrix4_old& Left, float Right) noexcept { return Left = Left * Right; }
inline Matrix4_old& operator*=(Matrix4_old& Left, const Matrix4_old& Right) noexcept { return Left = Left * Right; }
inline Matrix4_old& operator/=(Matrix4_old& Left, float Right) noexcept { return Left = Left / Right; }
inline Matrix4_old& operator/=(Matrix4_old& Left, const Matrix4_old& Right) noexcept { return Left = Left * Right.Inverse(); }