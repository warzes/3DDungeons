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

inline constexpr float DegreesToRadians(float degrees) noexcept { return degrees * DEG2RAD; }
inline constexpr float RadiansToDegrees(float radians) noexcept { return radians * RAD2DEG; }

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

inline Vector2 Rotate(const Vector2& v, float angle)
{
	const float cs = cosf(angle);
	const float sn = sinf(angle);
	return {
		v.x * cs - v.y * sn,
		v.x * sn + v.y * cs
	};
}

inline float Angle(const Vector2& x, const Vector2& y)
{
	return acos(Clamp(DotProduct(x, y), -1.0f, 1.0f));
}

inline float OrientedAngle(const Vector2& x, const Vector2& y)
{
	const float Angle(acos(Clamp(DotProduct(x, y), -1.0f, 1.0f)));
	const float partialCross = x.x * y.y - y.x * x.y;

	if( partialCross > 0.0f ) return Angle;
	else return -Angle;
}

inline Vector2 Slide(const Vector2& v, const Vector2& normal)
{
	const float d = DotProduct(v, normal);
	return v - normal * d; // проверить что так { v.x - normal.x * d, v.y - normal.y * d };
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
inline Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max) { return Max(Min(value, max), min); }
inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x) { return a + (b - a) * x; }

inline Vector3 SLerp(const Vector3 & x, const Vector3 & y, float a)
{
	// get cosine of angle between vectors (-1 -> 1)
	const float CosAlpha = DotProduct(x, y);
	// get angle (0 -> pi)
	const float Alpha = acos(CosAlpha);
	// get sine of angle between vectors (0 -> 1)
	const float SinAlpha = sin(Alpha);
	// this breaks down when SinAlpha = 0, i.e. Alpha = 0 or pi
	const float t1 = sin((1.0f - a) * Alpha) / SinAlpha;
	const float t2 = sin(a * Alpha) / SinAlpha;

	// interpolate src vectors
	return x * t1 + y * t2;
}

inline Vector3 Mix(const Vector3& x, const Vector3& y, float t) { return x * (1.0f - t) + y * t; }

inline Vector3 Rotate(const Vector3& v, float angle, const Vector3& normal)
{
	return Matrix3(Matrix4::Rotate(Matrix4::Identity, angle, normal)) * v;
}

inline Vector3 RotateX(const Vector3& v, float angle)
{
	const float Cos(cos(angle));
	const float Sin(sin(angle));

	Vector3 Result(v);
	Result.y = v.y * Cos - v.z * Sin;
	Result.z = v.y * Sin + v.z * Cos;
	return Result;
}

inline Vector3 RotateY(const Vector3& v, float angle)
{
	const float Cos(cos(angle));
	const float Sin(sin(angle));

	Vector3 Result(v);
	Result.x = v.x * Cos + v.z * Sin;
	Result.z = -v.x * Sin + v.z * Cos;
	return Result;
}

inline Vector3 RotateZ(const Vector3& v, float angle)
{
	const float Cos(cos(angle));
	const float Sin(sin(angle));

	Vector3 Result(v);
	Result.x = v.x * Cos - v.y * Sin;
	Result.y = v.x * Sin + v.y * Cos;
	return Result;
}

inline Vector3 Rotate(const Quaternion& q, const Vector3& v)
{
	return q * v;
}

inline float Angle(const Vector3& x, const Vector3& y)
{
	return acos(Clamp(DotProduct(x, y), -1.0f, 1.0f));
}

inline float OrientedAngle(const Vector3& x, const Vector3& y, const Vector3& ref)
{
	const float Angle(acos(Clamp(DotProduct(x, y), -1.0f, 1.0f)));
	return Mix(Angle, -Angle, DotProduct(ref, CrossProduct(x, y)) < 0.0f);
}

inline Vector3 Slide(const Vector3& v, const Vector3& normal)
{
	const float d = DotProduct(v, normal);
	return v - normal * d; //проверить что так { v.x - normal.x * d, v.y - normal.y * d, v.z - normal.z * d };
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

inline bool operator==(const Vector3& Left, const Vector3& Right) noexcept   { return Left.x == Right.x && Left.y == Right.y && Left.z == Right.z; }
inline bool operator!=(const Vector3& Left, const Vector3& Right) noexcept   { return Left.x != Right.x || Left.y != Right.y || Left.z != Right.z; }

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

inline Vector4 Rotate(const Vector4& v, float angle, const Vector3& normal)
{
	return Rotate(Matrix4::Identity, angle, normal) * v;
}

inline Vector4 RotateX(const Vector4& v, float angle)
{
	const float Cos(cos(angle));
	const float Sin(sin(angle));

	Vector4 Result = v;
	Result.y = v.y * Cos - v.z * Sin;
	Result.z = v.y * Sin + v.z * Cos;
	return Result;
}

inline Vector4 RotateY(const Vector4& v, float angle)
{
	const float Cos(cos(angle));
	const float Sin(sin(angle));

	Vector4 Result = v;
	Result.x = v.x * Cos + v.z * Sin;
	Result.z = -v.x * Sin + v.z * Cos;
	return Result;
}

inline Vector4 RotateZ(const Vector4& v, float angle)
{
	const float Cos(cos(angle));
	const float Sin(sin(angle));

	Vector4 Result = v;
	Result.x = v.x * Cos - v.y * Sin;
	Result.y = v.x * Sin + v.y * Cos;
	return Result;
}

inline Vector4 Rotate(const Quaternion& q, const Vector4& v)
{
	return q * v;
}

inline float Angle(const Vector4& x, const Vector4& y)
{
	return acos(Clamp(DotProduct(x, y), -1.0f, 1.0f));
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
	*this = Quaternion::ToQuaternion(m);
}

inline Quaternion::Quaternion(const Matrix4& m)
{
	*this = Quaternion::ToQuaternion(m);
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
	const float _y = 2.0f * (y * z + w * x);
	const float _x = w * w - x * x - y * y + z * z;

	if (Equals(Vector2(_x, _y), Vector2(0.0f), std::numeric_limits<float>::epsilon())) //avoid atan2(0,0) - handle singularity - Matiis
		return 2.0f * atan2(x, w);

	return atan2(_y, _x);
}

inline float Quaternion::RollAngle() const
{
	const float _y = 2.0f * (x * y + w * z);
	const float _x = w * w + x * x - y * y - z * z;

	if (Equals(Vector2(_x, _y), Vector2(0.0f), std::numeric_limits<float>::epsilon())) //avoid atan2(0,0) - handle singularity - Matiis
		return 0.0f;

	return atan2(_y, _x);
}

inline Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis)
{
	const float halfAngle = angle * 0.5f;
	const float Sin = sinf(halfAngle);
	return Quaternion(cosf(halfAngle), axis.x * Sin, axis.y * Sin, axis.z * Sin);
}

inline Quaternion Quaternion::FromEuler(const Vector3 & euler)
{
	const float ex = euler.x * 0.5f;
	const float ey = euler.y * 0.5f;
	const float ez = euler.z * 0.5f;
	const float sinX = sinf(ex);
	const float cosX = cosf(ex);
	const float sinY = sinf(ey);
	const float cosY = cosf(ey);
	const float sinZ = sinf(ez);
	const float cosZ = cosf(ez);
	Quaternion q;
	q.w = cosY * cosX * cosZ + sinY * sinX * sinZ;
	q.x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	q.y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	q.z = cosY * cosX * sinZ - sinY * sinX * cosZ;
	return q;
}

inline Quaternion Quaternion::ToQuaternion(const Matrix3& m)
{
	const float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	const float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	const float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
	const float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if( fourXSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if( fourYSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if( fourZSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	const float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	const float mult = 0.25f / biggestVal;

	switch( biggestIndex )
	{
	case 0: return { biggestVal, (m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult };
	case 1: return { (m[1][2] - m[2][1]) * mult, biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult };
	case 2: return { (m[2][0] - m[0][2]) * mult, (m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult };
	case 3: return { (m[0][1] - m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal };
	default:
		assert(false);
		return Quaternion::Identity;
	}
}

inline Quaternion Quaternion::ToQuaternion(const Matrix4& m)
{
	return ToQuaternion(Matrix3(m));
}

inline bool Equals(const Quaternion& v1, const Quaternion& v2, float epsilon) noexcept
{
	if (v1 == v2) return true;
	return ::Equals(v1.w, v2.w, epsilon)
		&& ::Equals(v1.x, v2.x, epsilon)
		&& ::Equals(v1.y, v2.y, epsilon)
		&& ::Equals(v1.z, v2.z, epsilon);
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

inline Quaternion CrossProduct(const Quaternion& q, const Vector3& v)
{
	return q * v;
}

inline Quaternion CrossProduct(const Vector3& v, const Quaternion& q)
{
	return q.Inverse() * v;
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
	const float Sin = sinf(halfAndgle);
	return q * Quaternion(cos(halfAndgle), tmp.x * Sin, tmp.y * Sin, tmp.z * Sin);
}

inline Quaternion QuatLookAt(const Vector3& direction, const Vector3& up)
{
	Matrix3 Result;
	Result[2] = direction;
	const Vector3& Right = CrossProduct(up, Result[2]);
	Result[0] = Right * 1.0f / sqrt(Max(0.00001f, DotProduct(Right, Right)));
	Result[1] = CrossProduct(Result[2], Result[0]);

	return Quaternion::ToQuaternion(Result);
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

inline Vector3 Matrix4::TransformPoint(const Vector3& pos) const
{
	return Vector3(
		value[0].x * pos.x + value[1].x * pos.y + value[2].x * pos.z + value[3].x,
		value[0].y * pos.x + value[1].y * pos.y + value[2].y * pos.z + value[3].y,
		value[0].z * pos.x + value[1].z * pos.y + value[2].z * pos.z + value[3].z);
}

inline bool Matrix4::Decompose(Vector3& scale, Quaternion& orientation, Vector3& translation, Vector3& skew, Vector4& perspective)
{
	Matrix4 LocalMatrix(*this);

	// Normalize the matrix.
	if ( fabsf(LocalMatrix[3][3]) < std::numeric_limits<float>::epsilon() )
		return false;

	for( size_t i = 0; i < 4; ++i )
		for( size_t j = 0; j < 4; ++j )
			LocalMatrix[i][j] /= LocalMatrix[3][3];

	// perspectiveMatrix is used to solve for perspective, but it also provides an easy way to test for singularity of the upper 3x3 component.
	Matrix4 PerspectiveMatrix(LocalMatrix);

	for( size_t i = 0; i < 3; i++ )
		PerspectiveMatrix[i][3] = 0.0f;
	PerspectiveMatrix[3][3] = 1.0f;

	if( fabsf(PerspectiveMatrix.GetDeterminant()) < std::numeric_limits<float>::epsilon() )
		return false;

	// First, isolate perspective.  This is the messiest.
	if(
		!fabsf(LocalMatrix[0][3]) >= std::numeric_limits<float>::epsilon() ||
		!fabsf(LocalMatrix[1][3]) >= std::numeric_limits<float>::epsilon() ||
		!fabsf(LocalMatrix[2][3]) >= std::numeric_limits<float>::epsilon() )
	{
		// rightHandSide is the right hand side of the equation.
		Vector4 RightHandSide;
		RightHandSide[0] = LocalMatrix[0][3];
		RightHandSide[1] = LocalMatrix[1][3];
		RightHandSide[2] = LocalMatrix[2][3];
		RightHandSide[3] = LocalMatrix[3][3];

		// Solve the equation by inverting PerspectiveMatrix and multiplying rightHandSide by the inverse.  (This is the easiest way, not necessarily the best.)
		Matrix4 InversePerspectiveMatrix = PerspectiveMatrix.Inverse();//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
		Matrix4 TransposedInversePerspectiveMatrix = InversePerspectiveMatrix.Transpose();//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);

		perspective = TransposedInversePerspectiveMatrix * RightHandSide;

		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0.0f;
		LocalMatrix[3][3] = 1.0f;
	}
	else
	{
		// No perspective.
		perspective = { 0, 0, 0, 1 };
	}

	// Next take care of translation (easy).
	translation = Vector3(LocalMatrix[3]);
	LocalMatrix[3] = Vector4(0, 0, 0, LocalMatrix[3].w);

	Vector3 Row[3], Pdum3;

	// Now get scale and shear.
	for( size_t i = 0; i < 3; ++i )
		for( size_t j = 0; j < 3; ++j )
			Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	scale.x = Row[0].GetLength();// v3Length(Row[0]);

	Row[0] = Row[0] * 1.0f / Row[0].GetLength();

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	skew.z = DotProduct(Row[0], Row[1]);
	Row[1] = (Row[1] * 1.0f) + (Row[0] * -skew.z);

	// Now, compute Y scale and normalize 2nd row.
	scale.y = Row[1].GetLength();
	Row[1] = Row[1] * 1.0f / Row[1].GetLength();
	skew.z /= scale.y;

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	skew.y = DotProduct(Row[0], Row[2]);
	Row[2] = (Row[2] * 1.0f) + (Row[0] * -skew.y);
	skew.x = DotProduct(Row[1], Row[2]);
	Row[2] = (Row[2] * 1.0f) + (Row[1] * -skew.x);

	// Next, get Z scale and normalize 3rd row.
	scale.z = Row[2].GetLength();
	Row[2] = Row[2] * 1.0f / Row[2].GetLength();
	skew.y /= scale.z;
	skew.x /= scale.z;

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	Pdum3 = CrossProduct(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if( DotProduct(Row[0], Pdum3) < 0 )
	{
		for( size_t i = 0; i < 3; i++ )
		{
			scale[i] *= -1.0f;
			Row[i] *= -1.0f;
		}
	}

	// Now, get the rotations out, as described in the gem.

	// FIXME - Add the ability to return either quaternions (which are
	// easier to recompose with) or Euler angles (rx, ry, rz), which
	// are easier for authors to deal with. The latter will only be useful
	// when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
	// will leave the Euler angle code here for now.

	// ret.rotateY = asin(-Row[0][2]);
	// if (cos(ret.rotateY) != 0) {
	//     ret.rotateX = atan2(Row[1][2], Row[2][2]);
	//     ret.rotateZ = atan2(Row[0][1], Row[0][0]);
	// } else {
	//     ret.rotateX = atan2(-Row[2][0], Row[1][1]);
	//     ret.rotateZ = 0;
	// }

	int i, j, k = 0;
	float root, trace = Row[0].x + Row[1].y + Row[2].z;
	if( trace > 0.0f )
	{
		root = sqrt(trace + 1.0f);
		orientation.w = 0.5f * root;
		root = 0.5f / root;
		orientation.x = root * (Row[1].z - Row[2].y);
		orientation.y = root * (Row[2].x - Row[0].z);
		orientation.z = root * (Row[0].y - Row[1].x);
	} // End if > 0
	else
	{
		static int Next[3] = { 1, 2, 0 };
		i = 0;
		if( Row[1].y > Row[0].x ) i = 1;
		if( Row[2].z > Row[i][i] ) i = 2;
		j = Next[i];
		k = Next[j];

		root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + 1.0f);
		const int off = 1;
		orientation[i + off] = 0.5f * root;
		root = 0.5f / root;
		orientation[j + off] = root * (Row[i][j] + Row[j][i]);
		orientation[k + off] = root * (Row[i][k] + Row[k][i]);
		orientation.w = root * (Row[j][k] - Row[k][j]);
	} // End if <= 0

	return true;
}

inline Matrix4 Matrix4::Translate(const Matrix4& m, const Vector3& v)
{
	Matrix4 Result(m);
	Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	return Result;
}

inline Matrix4 Matrix4::Rotate(const Matrix4& m, float angle, const Vector3& v)
{
	const float c = cos(angle);
	const float s = sin(angle);

	const Vector3 axis = v.GetNormalize();
	const Vector3 temp((1.0f - c) * axis);

	Matrix4 Rotate; // TODO: а может Matrix3? Последняя строка не используется
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

inline Matrix4 Matrix4::EulerAngleX(float angleX)
{
	const float cosX = cos(angleX);
	const float sinX = sin(angleX);

	return {
		1.0f,  0.0f, 0.0f, 0.0f,
		0.0f,  cosX, sinX, 0.0f,
		0.0f, -sinX, cosX, 0.0f,
		0.0f,  0.0f, 0.0f, 1.0f };
}

inline Matrix4 Matrix4::EulerAngleY(float angleY)
{
	const float cosY = cos(angleY);
	const float sinY = sin(angleY);

	return {
		cosY, 0.0f, -sinY, 0.0f,
		0.0f, 1.0f,  0.0f, 0.0f,
		sinY, 0.0f,  cosY, 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f };
}

inline Matrix4 Matrix4::EulerAngleZ(float angleZ)
{
	const float cosZ = cos(angleZ);
	const float sinZ = sin(angleZ);

	return {
		cosZ,  sinZ, 0.0f, 0.0f,
		-sinZ, cosZ, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 0.0f, 1.0f };
}

inline Matrix4 Matrix4::YawPitchRoll(float yaw, float pitch, float roll)
{
	const float tmp_ch = cos(yaw);
	const float tmp_sh = sin(yaw);
	const float tmp_cp = cos(pitch);
	const float tmp_sp = sin(pitch);
	const float tmp_cb = cos(roll);
	const float tmp_sb = sin(roll);

	Matrix4 Result;
	Result[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
	Result[0][1] = tmp_sb * tmp_cp;
	Result[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
	Result[0][3] = 0.0f;
	Result[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
	Result[1][1] = tmp_cb * tmp_cp;
	Result[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
	Result[1][3] = 0.0f;
	Result[2][0] = tmp_sh * tmp_cp;
	Result[2][1] = -tmp_sp;
	Result[2][2] = tmp_ch * tmp_cp;
	Result[2][3] = 0.0f;
	Result[3][0] = 0.0f;
	Result[3][1] = 0.0f;
	Result[3][2] = 0.0f;
	Result[3][3] = 1.0f;
	return Result;
}

inline Matrix4 Matrix4::Orientation(const Vector3& normal, const Vector3& up)
{
	if( (Equals(normal, up, std::numeric_limits<float>::epsilon())) )
		return Matrix4::Identity;

	Vector3 RotationAxis = CrossProduct(up, normal);
	const float Angle = acos(DotProduct(normal, up));

	return Rotate(Matrix4::Identity, Angle, RotationAxis);
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

	const float tanHalfFovy = tan(fovy * 0.5f);

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
	return Matrix4(CastToMatrix3(q));
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
// Transform
//=============================================================================

inline Transform Transform::Inverted() const
{
	Transform result;
	result.rotate = rotate.Conjugate();
	result.position = result.rotate * (-position / scale); // rotate
	result.scale = 1.0f / scale;
	return result;
}

inline Vector3 Transform::ToTransform(const Vector3 & value) const
{
	return position + (rotate*value) * scale;
}

inline Transform operator*(const Transform& Left, const Transform& Right) noexcept
{
	return { 
		Left.rotate * (Right.position * Left.scale) + Left.position,
		Left.rotate * Right.rotate,
		Left.scale * Right.scale };
}