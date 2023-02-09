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
	Set(f);
}

inline constexpr Matrix3::Matrix3(
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33)
{
	Set(m11, m12, m13, m21, m22, m23, m31, m32, m33);
}

inline constexpr void Matrix3::Set(const float* f)
{
	m[0] = f[0];
	m[1] = f[1];
	m[2] = f[2];
	m[3] = f[3];
	m[4] = f[4];
	m[5] = f[5];
	m[6] = f[6];
	m[7] = f[7];
	m[8] = f[8];
}

inline constexpr void Matrix3::Set(
	float m11, float m12, float m13, 
	float m21, float m22, float m23, 
	float m31, float m32, float m33)
{
	m[0] = m11;
	m[1] = m21;
	m[2] = m31;
	m[3] = m12;
	m[4] = m22;
	m[5] = m32;
	m[6] = m13;
	m[7] = m23;
	m[8] = m33;
}

inline constexpr void Matrix3::Set(const Matrix3& M)
{
	m[0] = M[0]; m[1] = M[1]; m[2] = M[2];
	m[3] = M[3]; m[4] = M[4]; m[5] = M[5];
	m[6] = M[6]; m[7] = M[7]; m[8] = M[8];
}

inline float Matrix3::GetDeterminant() const
{
	return
		  m[0] * m[4] * m[8]
		+ m[3] * m[7] * m[2]
		+ m[6] * m[1] * m[5]
		- m[0] * m[7] * m[5]
		- m[3] * m[1] * m[8]
		- m[6] * m[4] * m[2];
}

inline Matrix3 Matrix3::Transpose() const
{
	Matrix3 transposed;
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

inline void Matrix3::Scale(const Vector3& scale)
{
	m[0] *= scale.x;
	m[4] *= scale.y;
	m[8] *= scale.z;
}

inline Matrix3 Matrix3Scale(const Vector3& scale)
{
	Matrix3 ret;
	ret[0] = scale.x; ret[3] = 0.0f;    ret[6] = 0.0f;
	ret[1] = 0.0f;    ret[4] = scale.y; ret[7] = 0.0f;
	ret[2] = 0.0f;    ret[5] = 0.0f;    ret[8] = scale.z;
	return ret;
}

inline Matrix3 Matrix3RotationX(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix3 ret;
	ret[0] = 1.0f; ret[3] = 0.0f; ret[6] = 0.0f;
	ret[1] = 0.0f; ret[4] = c;    ret[7] = -s;
	ret[2] = 0.0f; ret[5] = s;    ret[8] = c;
	return ret;
}

inline Matrix3 Matrix3RotationY(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix3 ret;
	ret[0] = c;    ret[3] = 0.0f; ret[6] = s;
	ret[1] = 0.0f; ret[4] = 1.0f; ret[7] = 0.0f;
	ret[2] = -s;   ret[5] = 0.0f; ret[8] = c;
	return ret;
}

inline Matrix3 Matrix3RotationZ(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix3 ret;
	ret[0] = c;    ret[3] = -s;   ret[6] = 0.0f;
	ret[1] = s;    ret[4] = c;    ret[7] = 0.0f;
	ret[2] = 0.0f; ret[5] = 0.0f; ret[8] = 1.0f;
	return ret;
}

inline Matrix3 Matrix3Rotation(const Quaternion& q)
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
	Matrix3 ret;
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

inline Matrix3 operator*(float f, const Matrix3& m) noexcept
{
	return m * f;
}

inline Matrix3 operator*(const Matrix3& m, float f) noexcept
{
	// не нужно возвращать через конструктор, иначе поменяется порядок строк/столбцов. В будущем пофиксить
	Matrix3 result;
	result[0] = m[0] * f;
	result[1] = m[1] * f;
	result[2] = m[2] * f;
	result[3] = m[3] * f;
	result[4] = m[4] * f;
	result[5] = m[5] * f;
	result[6] = m[6] * f;
	result[7] = m[7] * f;
	result[8] = m[8] * f;
	return result;
}

inline Matrix3 operator*(const Matrix3& Left, const Matrix3& Right) noexcept
{
	Matrix3 multiplied;
	multiplied[0] = Left[0] * Right[0] + Left[3] * Right[1] + Left[6] * Right[2];
	multiplied[1] = Left[1] * Right[0] + Left[4] * Right[1] + Left[7] * Right[2];
	multiplied[2] = Left[2] * Right[0] + Left[5] * Right[1] + Left[8] * Right[2];
	multiplied[3] = Left[0] * Right[3] + Left[3] * Right[4] + Left[6] * Right[5];
	multiplied[4] = Left[1] * Right[3] + Left[4] * Right[4] + Left[7] * Right[5];
	multiplied[5] = Left[2] * Right[3] + Left[5] * Right[4] + Left[8] * Right[5];
	multiplied[6] = Left[0] * Right[6] + Left[3] * Right[7] + Left[6] * Right[8];
	multiplied[7] = Left[1] * Right[6] + Left[4] * Right[7] + Left[7] * Right[8];
	multiplied[8] = Left[2] * Right[6] + Left[5] * Right[7] + Left[8] * Right[8];
	return multiplied;
}

//=============================================================================
// Matrix4 Impl
//=============================================================================

inline constexpr Matrix4::Matrix4(const float* f)
{
	Set(f);
}

inline constexpr Matrix4::Matrix4(
	float m11, float m12, float m13, float m14, 
	float m21, float m22, float m23, float m24, 
	float m31, float m32, float m33, float m34, 
	float m41, float m42, float m43, float m44)
{
	Set(m11, m12, m13, m14, 
		m21, m22, m23, m24, 
		m31, m32, m33, m34, 
		m41, m42, m43, m44);
}

inline constexpr void Matrix4::Set(const float* f)
{
	m[ 0] = f[ 0]; m[ 1] = f[ 1]; m[ 2] = f[ 2]; m[ 3] = f[ 3];
	m[ 4] = f[ 4]; m[ 5] = f[ 5]; m[ 6] = f[ 6]; m[ 7] = f[ 7];
	m[ 8] = f[ 8]; m[ 9] = f[ 9]; m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr void Matrix4::Set(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	m[0] = m11;
	m[1] = m21;
	m[2] = m31;
	m[3] = m41;
	m[4] = m12;
	m[5] = m22;
	m[6] = m32;
	m[7] = m42;
	m[8] = m13;
	m[9] = m23;
	m[10] = m33;
	m[11] = m43;
	m[12] = m14;
	m[13] = m24;
	m[14] = m34;
	m[15] = m44;
}

inline constexpr void Matrix4::Set(const Matrix4& M)
{
	m[ 0] = M[ 0]; m[ 1] = M[ 1]; m[ 2] = M[ 2]; m[ 3] = M[ 3];
	m[ 4] = M[ 4]; m[ 5] = M[ 5]; m[ 6] = M[6 ]; m[ 7] = M[ 7];
	m[ 8] = M[ 8]; m[ 9] = M[ 9]; m[10] = M[10]; m[11] = M[11];
	m[12] = M[12]; m[13] = M[13]; m[14] = M[14]; m[15] = M[15];
}

inline void Matrix4::Scale(const Vector3 & scale)
{
	m[0] *= scale.x;
	m[5] *= scale.y;
	m[10] *= scale.z;
}

inline void Matrix4::Translate(const Vector3& pos)
{
	m[12] += pos.x;
	m[13] += pos.y;
	m[14] += pos.z;
}

inline float Matrix4::GetDeterminant() const
{
	// TODO: рефакт
	float m11 = m[0];
	float m21 = m[1];
	float m31 = m[2];
	float m41 = m[3];
	float m12 = m[4];
	float m22 = m[5];
	float m32 = m[6];
	float m42 = m[7];
	float m13 = m[8];
	float m23 = m[9];
	float m33 = m[10];
	float m43 = m[11];
	float m14 = m[12];
	float m24 = m[13];
	float m34 = m[14];
	float m44 = m[15];
	float determinant = 
		  m14 * m23 * m32 * m41 - m13 * m24 * m32 * m41
		- m14 * m22 * m33 * m41 + m12 * m24 * m33 * m41
		+ m13 * m22 * m34 * m41 - m12 * m23 * m34 * m41
		- m14 * m23 * m31 * m42 + m13 * m24 * m31 * m42
		+ m14 * m21 * m33 * m42 - m11 * m24 * m33 * m42
		- m13 * m21 * m34 * m42 + m11 * m23 * m34 * m42
		+ m14 * m22 * m31 * m43 - m12 * m24 * m31 * m43
		- m14 * m21 * m32 * m43 + m11 * m24 * m32 * m43
		+ m12 * m21 * m34 * m43 - m11 * m22 * m34 * m43
		- m13 * m22 * m31 * m44 + m12 * m23 * m31 * m44
		+ m13 * m21 * m32 * m44 - m11 * m23 * m32 * m44
		- m12 * m21 * m33 * m44 + m11 * m22 * m33 * m44;
	return determinant;
}

inline Matrix4 Matrix4::Transpose() const
{
	Matrix4 transposed;
	transposed[0] = m[0];
	transposed[1] = m[4];
	transposed[2] = m[8];
	transposed[3] = m[12];
	transposed[4] = m[1];
	transposed[5] = m[5];
	transposed[6] = m[9];
	transposed[7] = m[13];
	transposed[8] = m[2];
	transposed[9] = m[6];
	transposed[10] = m[10];
	transposed[11] = m[14];
	transposed[12] = m[3];
	transposed[13] = m[7];
	transposed[14] = m[11];
	transposed[15] = m[15];
	return transposed;
}

inline Matrix4 Matrix4::Inverse() const
{
	float m11 = m[0];
	float m21 = m[1];
	float m31 = m[2];
	float m41 = m[3];
	float m12 = m[4];
	float m22 = m[5];
	float m32 = m[6];
	float m42 = m[7];
	float m13 = m[8];
	float m23 = m[9];
	float m33 = m[10];
	float m43 = m[11];
	float m14 = m[12];
	float m24 = m[13];
	float m34 = m[14];
	float m44 = m[15];
	Matrix4 inverse;
	inverse[0] = m22 * m33 * m44
		- m22 * m43 * m34
		- m23 * m32 * m44
		+ m23 * m42 * m34
		+ m24 * m32 * m43
		- m24 * m42 * m33;
	inverse[4] = -m12 * m33 * m44
		+ m12 * m43 * m34
		+ m13 * m32 * m44
		- m13 * m42 * m34
		- m14 * m32 * m43
		+ m14 * m42 * m33;
	inverse[8] = m12 * m23 * m44
		- m12 * m43 * m24
		- m13 * m22 * m44
		+ m13 * m42 * m24
		+ m14 * m22 * m43
		- m14 * m42 * m23;
	inverse[12] = -m12 * m23 * m34
		+ m12 * m33 * m24
		+ m13 * m22 * m34
		- m13 * m32 * m24
		- m14 * m22 * m33
		+ m14 * m32 * m23;
	inverse[1] = -m21 * m33 * m44
		+ m21 * m43 * m34
		+ m23 * m31 * m44
		- m23 * m41 * m34
		- m24 * m31 * m43
		+ m24 * m41 * m33;
	inverse[5] = m11 * m33 * m44
		- m11 * m43 * m34
		- m13 * m31 * m44
		+ m13 * m41 * m34
		+ m14 * m31 * m43
		- m14 * m41 * m33;
	inverse[9] = -m11 * m23 * m44
		+ m11 * m43 * m24
		+ m13 * m21 * m44
		- m13 * m41 * m24
		- m14 * m21 * m43
		+ m14 * m41 * m23;
	inverse[13] = m11 * m23 * m34
		- m11 * m33 * m24
		- m13 * m21 * m34
		+ m13 * m31 * m24
		+ m14 * m21 * m33
		- m14 * m31 * m23;
	inverse[2] = m21 * m32 * m44
		- m21 * m42 * m34
		- m22 * m31 * m44
		+ m22 * m41 * m34
		+ m24 * m31 * m42
		- m24 * m41 * m32;
	inverse[6] = -m11 * m32 * m44
		+ m11 * m42 * m34
		+ m12 * m31 * m44
		- m12 * m41 * m34
		- m14 * m31 * m42
		+ m14 * m41 * m32;
	inverse[10] = m11 * m22 * m44
		- m11 * m42 * m24
		- m12 * m21 * m44
		+ m12 * m41 * m24
		+ m14 * m21 * m42
		- m14 * m41 * m22;
	inverse[14] = -m11 * m22 * m34
		+ m11 * m32 * m24
		+ m12 * m21 * m34
		- m12 * m31 * m24
		- m14 * m21 * m32
		+ m14 * m31 * m22;
	inverse[3] = -m21 * m32 * m43
		+ m21 * m42 * m33
		+ m22 * m31 * m43
		- m22 * m41 * m33
		- m23 * m31 * m42
		+ m23 * m41 * m32;
	inverse[7] = m11 * m32 * m43
		- m11 * m42 * m33
		- m12 * m31 * m43
		+ m12 * m41 * m33
		+ m13 * m31 * m42
		- m13 * m41 * m32;
	inverse[11] = -m11 * m22 * m43
		+ m11 * m42 * m23
		+ m12 * m21 * m43
		- m12 * m41 * m23
		- m13 * m21 * m42
		+ m13 * m41 * m22;
	inverse[15] = m11 * m22 * m33
		- m11 * m32 * m23
		- m12 * m21 * m33
		+ m12 * m31 * m23
		+ m13 * m21 * m32
		- m13 * m31 * m22;
	float inverted_determinant = 1.0f / (m11 * inverse[0] + m21 * inverse[4] + m31 * inverse[8] + m41 * inverse[12]);
	Matrix4 result;
	result[0] = inverse[0] * inverted_determinant;
	result[1] = inverse[1] * inverted_determinant;
	result[2] = inverse[2] * inverted_determinant;
	result[3] = inverse[3] * inverted_determinant;
	result[4] = inverse[4] * inverted_determinant;
	result[5] = inverse[5] * inverted_determinant;
	result[6] = inverse[6] * inverted_determinant;
	result[7] = inverse[7] * inverted_determinant;
	result[8] = inverse[8] * inverted_determinant;
	result[9] = inverse[9] * inverted_determinant;
	result[10] = inverse[10] * inverted_determinant;
	result[11] = inverse[11] * inverted_determinant;
	result[12] = inverse[12] * inverted_determinant;
	result[13] = inverse[13] * inverted_determinant;
	result[14] = inverse[14] * inverted_determinant;
	result[15] = inverse[15] * inverted_determinant;
	return result;
}

inline Matrix4 Cofactor(const Matrix3& m0)
{
	Matrix4 cofactor;
	Matrix3 minor;
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

inline Matrix4 Matrix4Scale(const Vector3& scale)
{
	Matrix4 mat;
	mat[0] = scale.x; mat[4] = 0.0f;    mat[ 8] = 0.0f;    mat[12] = 0.0f;
	mat[1] = 0.0f;    mat[5] = scale.y; mat[ 9] = 0.0f;    mat[13] = 0.0f;
	mat[2] = 0.0f;    mat[6] = 0.0f;    mat[10] = scale.z; mat[14] = 0.0f;
	mat[3] = 0.0f;    mat[7] = 0.0f;    mat[11] = 0.0f;    mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4RotationX(float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);

	Matrix4 ret;
	ret[0] = 1.0f; ret[4] = 0.0f; ret[ 8] = 0.0f; ret[12] = 0.0f;
	ret[1] = 0.0f; ret[5] = c;    ret[ 9] = -s;   ret[13] = 0.0f;
	ret[2] = 0.0f; ret[6] = s;    ret[10] = c;    ret[14] = 0.0f;
	ret[3] = 0.0f; ret[7] = 0.0f; ret[11] = 0.0f; ret[15] = 1.0f;
	return ret;
}

inline Matrix4 Matrix4RotationY(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4 mat;
	mat[0] = c;    mat[4] = 0.0f; mat[ 8] = s;    mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[ 9] = 0.0f; mat[13] = 0.0f;
	mat[2] = -s;   mat[6] = 0.0f; mat[10] = c;    mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4RotationZ(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4 mat;
	mat[0] = c;    mat[4] = -s;   mat[ 8] = 0.0f; mat[12] = 0.0f;
	mat[1] = s;    mat[5] = c;    mat[ 9] = 0.0f; mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4Rotation(const Vector3& axis, float angle)
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

	Matrix4 mat;
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

inline Matrix4 Matrix4Rotation(const Quaternion& q0)
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
	Matrix4 mat;
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

inline Matrix4 Matrix4Translate(const Vector3 & v)
{
	Matrix4 mat;
	mat[0] = 1.0f; mat[4] = 0.0f; mat[ 8] = 0.0f; mat[12] = v.x;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[ 9] = 0.0f; mat[13] = v.y;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = v.z;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4 LookAt(const Vector3& eye, const Vector3& dir, const Vector3& up)
{
	Vector3 tmp_forward = (dir - eye).GetNormalize();
	Vector3 tmp_side = CrossProduct(tmp_forward, up).GetNormalize();
	Vector3 tmp_up = CrossProduct(tmp_side, tmp_forward);
	Matrix4 m0;
	m0[0] = tmp_side.x;     m0[4] = tmp_side.y;     m0[ 8] = tmp_side.z;     m0[12] = -DotProduct(tmp_side, eye);
	m0[1] = tmp_up.x;       m0[5] = tmp_up.y;       m0[ 9] = tmp_up.z;       m0[13] = -DotProduct(tmp_up, eye);
	m0[2] = -tmp_forward.x; m0[6] = -tmp_forward.y; m0[10] = -tmp_forward.z; m0[14] = -DotProduct(tmp_forward, eye);
	m0[3] = 0.0f;           m0[7] = 0.0f;           m0[11] = 0.0f;           m0[15] = 1.0f;
}

inline Matrix4 Ortho(float left, float right, float bottom, float top, float n, float f)
{
	Matrix4 result;
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

inline Matrix4 Perspective(float fov_y, float aspect, float n, float f)
{
	const float tan_half_fov_y = 1.0f / tanf(fov_y * 0.5f);
	Matrix4 mat;
	mat[ 0] = 1.0f / aspect * tan_half_fov_y;
	mat[ 1] = 0.0f;
	mat[ 2] = 0.0f;
	mat[ 3] = 0.0f;
	mat[ 4] = 0.0f;
	mat[ 5] = 1.0f / tan_half_fov_y;
	mat[ 6] = 0.0f;
	mat[ 7] = 0.0f;
	mat[ 8] = 0.0f;
	mat[ 9] = 0.0f;
	mat[10] = f / (n - f);
	mat[11] = -1.0f;
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = -(f * n) / (f - n);
	mat[15] = 0.0f;
	return mat;
}

inline Matrix4 operator*(float f, const Matrix4& m) noexcept
{
	return m * f;
}

inline Matrix4 operator*(const Matrix4& m, float f) noexcept
{
	// не нужно возвращать через конструктор, иначе поменяется порядок строк/столбцов. В будущем пофиксить
	Matrix4 result;
	result[0] = m[0] * f;
	result[1] = m[1] * f;
	result[2] = m[2] * f;
	result[3] = m[3] * f;
	result[4] = m[4] * f;
	result[5] = m[5] * f;
	result[6] = m[6] * f;
	result[7] = m[7] * f;
	result[8] = m[8] * f;
	result[9] = m[9] * f;
	result[10] = m[10] * f;
	result[11] = m[11] * f;
	result[12] = m[12] * f;
	result[13] = m[13] * f;
	result[14] = m[14] * f;
	result[15] = m[15] * f;
	return result;
}

inline Matrix4 operator*(const Matrix4& m0, const Matrix4& m1) noexcept
{
	Matrix4 multiplied;
	multiplied[0] = m0[0] * m1[0] + m0[4] * m1[1] + m0[8] * m1[2] + m0[12] * m1[3];
	multiplied[1] = m0[1] * m1[0] + m0[5] * m1[1] + m0[9] * m1[2] + m0[13] * m1[3];
	multiplied[2] = m0[2] * m1[0] + m0[6] * m1[1] + m0[10] * m1[2] + m0[14] * m1[3];
	multiplied[3] = m0[3] * m1[0] + m0[7] * m1[1] + m0[11] * m1[2] + m0[15] * m1[3];
	multiplied[4] = m0[0] * m1[4] + m0[4] * m1[5] + m0[8] * m1[6] + m0[12] * m1[7];
	multiplied[5] = m0[1] * m1[4] + m0[5] * m1[5] + m0[9] * m1[6] + m0[13] * m1[7];
	multiplied[6] = m0[2] * m1[4] + m0[6] * m1[5] + m0[10] * m1[6] + m0[14] * m1[7];
	multiplied[7] = m0[3] * m1[4] + m0[7] * m1[5] + m0[11] * m1[6] + m0[15] * m1[7];
	multiplied[8] = m0[0] * m1[8] + m0[4] * m1[9] + m0[8] * m1[10] + m0[12] * m1[11];
	multiplied[9] = m0[1] * m1[8] + m0[5] * m1[9] + m0[9] * m1[10] + m0[13] * m1[11];
	multiplied[10] = m0[2] * m1[8] + m0[6] * m1[9] + m0[10] * m1[10] + m0[14] * m1[11];
	multiplied[11] = m0[3] * m1[8] + m0[7] * m1[9] + m0[11] * m1[10] + m0[15] * m1[11];
	multiplied[12] = m0[0] * m1[12] + m0[4] * m1[13] + m0[8] * m1[14] + m0[12] * m1[15];
	multiplied[13] = m0[1] * m1[12] + m0[5] * m1[13] + m0[9] * m1[14] + m0[13] * m1[15];
	multiplied[14] = m0[2] * m1[12] + m0[6] * m1[13] + m0[10] * m1[14] + m0[14] * m1[15];
	multiplied[15] = m0[3] * m1[12] + m0[7] * m1[13] + m0[11] * m1[14] + m0[15] * m1[15];
	return multiplied;
}