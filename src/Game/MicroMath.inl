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

inline float Vector2::GetAngle() const { return atan2f(y, x); }
inline float Vector2::GetLength() const { return sqrtf(x * x + y * y); }
inline float Vector2::GetLengthSquared() const { return x * x + y * y; }
inline Vector2 Vector2::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen };
}

inline bool Equals(const Vector2& v1, const Vector2& v2, float epsilon) noexcept 
{
	if( v1 == v2 ) return true;
	return Equals(v1.x, v2.x, epsilon)
		&& Equals(v1.y, v2.y, epsilon);
}
inline Vector2 Min(const Vector2& v1, const Vector2& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y) }; }
inline Vector2 Max(const Vector2& v1, const Vector2& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y) }; }
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x) { return a + (b - a) * x; }
inline Vector2 Mix(const Vector2& u, const Vector2& v, float a) { return u * (1.0f - a) + v * a; }

inline float Distance(const Vector2& v1, const Vector2& v2) { return (v1 - v2).GetLength(); }
inline float DotProduct(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
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

inline Vector2 Reflect(const Vector2& v, const Vector2& normal)
{
	const float d = 2.0f * DotProduct(v, normal);
	return normal * d - v; // проверить что так  { normal.x * d - v.x, normal.y * d - v.y };
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

inline Vector2 Bezier4(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2, const Vector2 & v3, float f)
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

inline float Vector3::GetLength() const { return sqrtf(x * x + y * y + z * z); }
inline float Vector3::GetLengthSquared() const { return x * x + y * y + z * z; }
inline Vector3 Vector3::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen, z * invLen };
}

inline bool Equals(const Vector3& v1, const Vector3& v2, float epsilon) noexcept
{
	if( v1 == v2 ) return true;
	return Equals(v1.x, v2.x, epsilon)
		&& Equals(v1.y, v2.y, epsilon)
		&& Equals(v1.z, v2.z, epsilon);
}
inline Vector3 Min(const Vector3& v1, const Vector3& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z) }; }
inline Vector3 Max(const Vector3& v1, const Vector3& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z) }; }
inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x) { return a + (b - a) * x; }
inline Vector3 Mix(const Vector3& u, const Vector3& v, float t) { return u * (1.0f - t) + v * t; }

inline float Distance(const Vector3& v1, const Vector3& v2) { return (v1 - v2).GetLength(); }
inline float DotProduct(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2) 
{
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
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
inline Vector3 operator/(float Left, const Vector3& Right) noexcept          { return {   Left / Right.x,   Left / Right.y,   Left / Right.z }; }
inline Vector3 operator/(const Vector3& Left, float Right) noexcept          { return { Left.x / Right,   Left.y / Right,   Left.z / Right   }; }
inline Vector3 operator/(const Vector3& Left, const Vector3& Right) noexcept { return { Left.x / Right.x, Left.y / Right.y, Left.z / Right.z }; }

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
	if( v1 == v2 ) return true;
	return Equals(v1.x, v2.x, epsilon)
		&& Equals(v1.y, v2.y, epsilon)
		&& Equals(v1.z, v2.z, epsilon)
		&& Equals(v1.w, v2.w, epsilon);
}
inline float DotProduct(const Vector4& v1, const Vector4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline bool operator==(const Vector4& Left, const Vector4& Right) noexcept { return Left.x == Right.x && Left.y == Right.y && Left.z == Right.z && Left.w == Right.w; }

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
inline Vector4 operator/(float Left, const Vector4& Right) noexcept          { return {   Left / Right.x,   Left / Right.y,   Left / Right.z,   Left / Right.w }; }
inline Vector4 operator/(const Vector4& Left, float Right) noexcept          { return { Left.x / Right,   Left.y / Right,   Left.z / Right,   Left.w / Right   }; }
inline Vector4 operator/(const Vector4& Left, const Vector4& Right) noexcept { return { Left.x / Right.x, Left.y / Right.y, Left.z / Right.z, Left.w / Right.w }; }

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

inline float Quaternion::GetLength() const { return sqrtf(x * x + y * y + z * z + w * w); }
inline float Quaternion::GetLengthSquared() const { return x * x + y * y + z * z + w * w; }
inline Quaternion Quaternion::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen, z * invLen, w * invLen };
}

inline bool Equals(const Quaternion& v1, const Quaternion& v2, float epsilon) noexcept
{
	if( v1 == v2 ) return true;
	return Equals(v1.x, v2.x, epsilon) 
		&& Equals(v1.y, v2.y, epsilon) 
		&& Equals(v1.z, v2.z, epsilon) 
		&& Equals(v1.w, v2.w, epsilon);
}
inline float DotProduct(const Quaternion& v1, const Quaternion& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline bool operator==(const Quaternion& Left, const Quaternion& Right) noexcept { return Left.x == Right.x && Left.y == Right.y && Left.z == Right.z && Left.w == Right.w; }

inline Quaternion operator-(const Quaternion& In) noexcept                            { return { -In.x, -In.y, -In.z, -In.w }; }
inline Quaternion operator-(const Quaternion& Left, const Quaternion& Right) noexcept { return { Left.x - Right.x, Left.y - Right.y, Left.z - Right.z, Left.w - Right.w }; }
inline Quaternion operator+(const Quaternion& Left, const Quaternion& Right) noexcept { return { Left.x + Right.x, Left.y + Right.y, Left.z + Right.z, Left.w + Right.w }; }
inline Quaternion operator*(float Left, const Quaternion& Right) noexcept             { return {   Left * Right.x,   Left * Right.y,   Left * Right.z,   Left * Right.w }; }
inline Quaternion operator*(const Quaternion& Left, float Right) noexcept             { return { Left.x * Right,   Left.y * Right,   Left.z * Right,   Left.w * Right   }; }
//inline Quaternion operator*(const Quaternion& Left, const Quaternion& Right) noexcept 
//{ 
//
//	//кватернионы умножаются по своему
//}
//inline Quaternion operator/(const Quaternion& Left, const Quaternion& Right) noexcept
//{
//	//кватернионы делятся по своему
//}

inline Quaternion& operator-=(Quaternion& Left, const Quaternion& Right) noexcept { return Left = Left - Right; }
inline Quaternion& operator+=(Quaternion& Left, const Quaternion& Right) noexcept { return Left = Left + Right; }
inline Quaternion& operator*=(Quaternion& Left, float Right) noexcept { return Left = Left * Right; }
inline Quaternion& operator*=(Quaternion& Left, const Quaternion& Right) noexcept { return Left = Left * Right; }

//=============================================================================
// Matrix3 Impl
//=============================================================================

inline constexpr Matrix3::Matrix3(const float* f)
{
	m[0] = f[0]; m[1] = f[1]; m[2] = f[2];
	m[3] = f[3]; m[4] = f[4]; m[5] = f[5];
	m[6] = f[6]; m[7] = f[7]; m[8] = f[8];
}

inline constexpr Matrix3::Matrix3(
	float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	m[0] = m0; m[1] = m1; m[2] = m2;
	m[3] = m3; m[4] = m4; m[5] = m5;
	m[6] = m6; m[7] = m7; m[8] = m8;
}

inline constexpr void Matrix3::Set(const float* f)
{
	m[0] = f[0]; m[1] = f[1]; m[2] = f[2];
	m[3] = f[3]; m[4] = f[4]; m[5] = f[5];
	m[6] = f[6]; m[7] = f[7]; m[8] = f[8];
}

inline constexpr void Matrix3::Set(
	float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	m[0] = m0; m[1] = m1; m[2] = m2;
	m[3] = m3; m[4] = m4; m[5] = m5;
	m[6] = m6; m[7] = m7; m[8] = m8;
}

inline constexpr void Matrix3::Set(const Matrix3& M)
{
	m[0] = M[0]; m[1] = M[1]; m[2] = M[2];
	m[3] = M[3]; m[4] = M[4]; m[5] = M[5];
	m[6] = M[6]; m[7] = M[7]; m[8] = M[8];
}

//=============================================================================
// Matrix4 Impl
//=============================================================================

inline constexpr Matrix4::Matrix4(const float* f)
{
	m[ 0] = f[ 0]; m[ 1] = f[ 1]; m[ 2] = f[ 2]; m[ 3] = f[ 3];
	m[ 4] = f[ 4]; m[ 5] = f[ 5]; m[ 6] = f[ 6]; m[ 7] = f[ 7];
	m[ 8] = f[ 8]; m[ 9] = f[ 9]; m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr Matrix4::Matrix4(
	float  m0, float  m1, float  m2, float  m3,
	float  m4, float  m5, float  m6, float  m7,
	float  m8, float  m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[ 0] = m0;  m[ 1] = m1;  m[ 2] = m2;  m[ 3] = m3;
	m[ 4] = m4;  m[ 5] = m5;  m[ 6] = m6;  m[ 7] = m7;
	m[ 8] = m8;  m[ 9] = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

inline constexpr void Matrix4::Set(const float* f)
{
	m[ 0] = f[ 0]; m[ 1] = f[ 1]; m[ 2] = f[ 2]; m[ 3] = f[ 3];
	m[ 4] = f[ 4]; m[ 5] = f[ 5]; m[ 6] = f[ 6]; m[ 7] = f[ 7];
	m[ 8] = f[ 8]; m[ 9] = f[ 9]; m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr void Matrix4::Set(
	float  m0, float  m1, float  m2, float  m3,
	float  m4, float  m5, float  m6, float  m7,
	float  m8, float  m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[ 0] = m0;  m[ 1] = m1;  m[ 2] = m2;  m[ 3] = m3;
	m[ 4] = m4;  m[ 5] = m5;  m[ 6] = m6;  m[ 7] = m7;
	m[ 8] = m8;  m[ 9] = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

inline constexpr void Matrix4::Set(const Matrix4& M)
{
	m[ 0] = M[ 0]; m[ 1] = M[ 1]; m[ 2] = M[ 2]; m[ 3] = M[ 3];
	m[ 4] = M[ 4]; m[ 5] = M[ 5]; m[ 6] = M[6 ]; m[ 7] = M[ 7];
	m[ 8] = M[ 8]; m[ 9] = M[ 9]; m[10] = M[10]; m[11] = M[11];
	m[12] = M[12]; m[13] = M[13]; m[14] = M[14]; m[15] = M[15];
}