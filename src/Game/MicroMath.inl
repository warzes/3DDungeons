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

inline Vector3 Reflect(const Vector3& v, const Vector3& normal)
{
	const float d = 2.0f * DotProduct(v, normal);
	return normal * d - v; // проверить что так  { normal.x * d - v.x, normal.y * d - v.y, normal.z * d - v.z };
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
inline Vector3 operator*(const Matrix3& m, const Vector3& v) noexcept
{
	return {
		m[0] * v.x + m[3] * v.y + m[6] * v.z,
		m[1] * v.x + m[4] * v.y + m[7] * v.z,
		m[2] * v.x + m[5] * v.y + m[8] * v.z
	};
}

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
inline Vector4 Min(const Vector4& v1, const Vector4& v2) { return { Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z), Min(v1.w, v2.w) }; }
inline Vector4 Max(const Vector4& v1, const Vector4& v2) { return { Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z), Max(v1.w, v2.w) }; }
inline Vector4 Lerp(const Vector4& a, const Vector4& b, float x) { return a + (b - a) * x; }
inline float DotProduct(const Vector4& v1, const Vector4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

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
inline Vector4 operator*(const Matrix4& m, const Vector4& v) noexcept
{
	return {
		m[0] * v.x + m[4] * v.y + m[ 8] * v.z + m[12] * v.w,
		m[1] * v.x + m[5] * v.y + m[ 9] * v.z + m[13] * v.w,
		m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
		m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
	};
}
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

const inline Quaternion Quaternion::Identity = { 0.0f, 0.0f, 0.0f, 1.0f };

inline Quaternion::Quaternion(float angle, const Vector3& axis)
{
	FromAngleAxis(angle, axis);
}

inline Quaternion::Quaternion(float ax, float ay, float az)
{
	FromEulerAngles(x, y, z);
}

inline Quaternion::Quaternion(const Vector3& start, const Vector3& end)
{
	FromRotationTo(start, end);
}

inline Quaternion::Quaternion(const Matrix4& m)
{
	FromMatrix(m);
}

inline void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
{
	Vector3 normAxis = axis.GetNormalize();
	const float half = angle * 0.5f;
	const float sinAngle = sinf(half);
	const float cosAngle = cosf(half);
	x = normAxis.x * sinAngle;
	y = normAxis.y * sinAngle;
	z = normAxis.z * sinAngle;
	w = cosAngle;
}

inline void Quaternion::FromEulerAngles(float x_, float y_, float z_)
{
	// Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
	x_ *= 0.5f;
	y_ *= 0.5f;
	z_ *= 0.5f;
	const float sinX = sinf(x_);
	const float cosX = cosf(x_);
	const float sinY = sinf(y_);
	const float cosY = cosf(y_);
	const float sinZ = sinf(z_);
	const float cosZ = cosf(z_);

	x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	z = cosY * cosX * sinZ - sinY * sinX * cosZ;
	w = cosY * cosX * cosZ + sinY * sinX * sinZ;
}

inline void Quaternion::FromRotationTo(const Vector3& start, const Vector3& end)
{
	const Vector3 normStart = start.GetNormalize();
	const Vector3 normEnd = end.GetNormalize();
	const float d = DotProduct(normStart, normEnd);

	if( d > -1.0f + EPSILON )
	{
		const Vector3 c = CrossProduct(normStart, normEnd);
		const float s = sqrtf((1.0f + d) * 2.0f);
		const float invS = 1.0f / s;

		x = c.x * invS;
		y = c.y * invS;
		z = c.z * invS;
		w = 0.5f * s;
	}
	else
	{
		Vector3 axis = CrossProduct(Vector3::Right, normStart);
		if( axis.GetLength() < EPSILON )
			axis = CrossProduct(Vector3::Up, normStart);
		FromAngleAxis(180.0f*DEG2RAD, axis);
	}
}

inline void Quaternion::FromMatrix(const Matrix4& m0)
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

inline float Quaternion::GetLength() const { return sqrtf(x * x + y * y + z * z + w * w); }
inline float Quaternion::GetLengthSquared() const { return x * x + y * y + z * z + w * w; }
inline Quaternion Quaternion::GetNormalize() const
{
	const float invLen = 1.0f / GetLength();
	return { x * invLen, y * invLen, z * invLen, w * invLen };
}

inline Quaternion Quaternion::Inverse() const
{
	const float lenSquared = GetLengthSquared();
	if( lenSquared == 1.0f ) return Conjugate();
	else if( lenSquared >= EPSILON ) return Conjugate() * (1.0f / lenSquared);
	else return Identity;
}

inline Vector3 Quaternion::EulerAngles() const
{
	// Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
	// Order of rotations: Z first, then X, then Y
	const float check = 2.0f * (w * x - y * z);

	if( check < -0.995f )
	{
		return Vector3(
			-90.0f*DEG2RAD,
			0.0f,
			-atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
		);
	}
	else if( check > 0.995f )
	{
		return Vector3(
			90.0f * DEG2RAD,
			0.0f,
			atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
		);
	}
	else
	{
		return Vector3(
			asinf(check),
			atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)),
			atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z))
		);
	}
}

inline float Quaternion::YawAngle() const
{
	return EulerAngles().y;
}

inline float Quaternion::PitchAngle() const
{
	return EulerAngles().x;
}

inline float Quaternion::RollAngle() const
{
	return EulerAngles().z;
}

inline bool Equals(const Quaternion& v1, const Quaternion& v2, float epsilon) noexcept
{
	if( v1 == v2 ) return true;
	return Equals(v1.x, v2.x, epsilon) 
		&& Equals(v1.y, v2.y, epsilon) 
		&& Equals(v1.z, v2.z, epsilon) 
		&& Equals(v1.w, v2.w, epsilon);
}

inline Quaternion Lerp(const Quaternion& a, const Quaternion& b, float x)
{
	return a + (b - a) * x;
}

inline Quaternion SLerp(const Quaternion& q0, const Quaternion& q1, float f)
{
	float d = DotProduct(q0, q1);
	Quaternion tmp1 = q1;
	float f0;
	float f1;
	if( d < 0.0f )
	{
		tmp1 = -tmp1;
		d = -d;
	}

	if( d > 0.9995f )
	{
		f0 = 1.0f - f;
		f1 = f;
	}
	else
	{
		float theta = acosf(d);
		float sin_theta = sinf(theta);
		f0 = sinf((1.0f - f) * theta) / sin_theta;
		f1 = sinf(f * theta) / sin_theta;
	}
	return {
		q0[0] * f0 + tmp1[0] * f1,
		q0[1] * f0 + tmp1[1] * f1,
		q0[2] * f0 + tmp1[2] * f1,
		q0[3] * f0 + tmp1[3] * f1
	};
}

inline float DotProduct(const Quaternion& v1, const Quaternion& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline Quaternion QuatPower(const Quaternion& in, const Quaternion& q0, float exponent)
{
	if( fabsf(q0[3]) < 1.0f - EPSILON )
	{
		const float alpha = acosf(q0[3]);
		const float new_alpha = alpha * exponent;
		const float s = sinf(new_alpha) / sinf(alpha);
		return { in.x * s, in.y * s, in.z * s, cosf(new_alpha) };
	}
	else
		return q0;
}

inline bool operator==(const Quaternion& Left, const Quaternion& Right) noexcept { return Left.x == Right.x && Left.y == Right.y && Left.z == Right.z && Left.w == Right.w; }

inline Quaternion operator-(const Quaternion& In) noexcept                            { return { -In.x, -In.y, -In.z, -In.w }; }
inline Quaternion operator-(const Quaternion& Left, const Quaternion& Right) noexcept { return { Left.x - Right.x, Left.y - Right.y, Left.z - Right.z, Left.w - Right.w }; }
inline Quaternion operator+(const Quaternion& Left, const Quaternion& Right) noexcept { return { Left.x + Right.x, Left.y + Right.y, Left.z + Right.z, Left.w + Right.w }; }
inline Quaternion operator*(float Left, const Quaternion& Right) noexcept             { return {   Left * Right.x,   Left * Right.y,   Left * Right.z,   Left * Right.w }; }
inline Quaternion operator*(const Quaternion& Left, float Right) noexcept             { return { Left.x * Right,   Left.y * Right,   Left.z * Right,   Left.w * Right   }; }
inline Quaternion operator*(const Quaternion& Left, const Quaternion& Right) noexcept 
{ 
	return {
		Left.w * Right.x + Left.x * Right.w + Left.y * Right.z - Left.z * Right.y,
		Left.w * Right.y + Left.y * Right.w + Left.z * Right.x - Left.x * Right.z,
		Left.w * Right.z + Left.z * Right.w + Left.x * Right.y - Left.y * Right.x,
		Left.w * Right.w - Left.x * Right.x - Left.y * Right.y - Left.z * Right.z
	};
}
inline Quaternion operator/(const Quaternion& Left, float Right) noexcept { return { Left.x / Right,   Left.y / Right,   Left.z / Right,   Left.w / Right }; }
inline Quaternion operator/(const Quaternion& Left, const Quaternion& Right) noexcept
{
	const float ls = Right[0] * Right[0] + Right[1] * Right[1] + Right[2] * Right[2] + Right[3] * Right[3];
	const float normalized_x = -Right[0] / ls;
	const float normalized_y = -Right[1] / ls;
	const float normalized_z = -Right[2] / ls;
	const float normalized_w =  Right[3] / ls;
	return {
		Left.x * normalized_w + normalized_x * Left.w + (Left.y * normalized_z - Left.z * normalized_y),
		Left.y * normalized_w + normalized_y * Left.w + (Left.z * normalized_x - Left.x * normalized_z),
		Left.z * normalized_w + normalized_z * Left.w + (Left.x * normalized_y - Left.y * normalized_x),
		Left.w * normalized_w - (Left.x * normalized_x + Left.y * normalized_y + Left.z * normalized_z)
	};
}

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