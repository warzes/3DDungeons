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
	constexpr Point2() = default;
	constexpr Point2(Point2&&) = default;
	constexpr Point2(const Point2&) = default;
	constexpr Point2(int n) : x(n), y(n) {}
	constexpr Point2(int nx, int ny) : x(nx), y(ny) {}

	constexpr Point2& operator=(Point2&&) = default;
	constexpr Point2& operator=(const Point2&) = default;

	int x = 0;
	int y = 0;
};

//=============================================================================
// Color
//=============================================================================
class Color
{
public:
	constexpr Color() = default;
	constexpr Color(Color&&) = default;
	constexpr Color(const Color&) = default;
	constexpr Color(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na = 255) : r(nr), g(ng), b(nb), a(na) {}

	constexpr Color& operator=(Color&&) = default;
	constexpr Color& operator=(const Color&) = default;

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
	constexpr Vector2() = default;
	constexpr Vector2(Vector2&&) = default;
	constexpr Vector2(const Vector2&) = default;
	constexpr Vector2(float num) : x(num), y(num) {}
	constexpr Vector2(float nx, float ny) : x(nx), y(ny) {}

	constexpr Vector2& operator=(Vector2&&) = default;
	constexpr Vector2& operator=(const Vector2&) = default;

	friend Vector2 operator+(const Vector2& u, float num) { return { u.x + num, u.y + num }; }
	friend Vector2 operator+(float num, const Vector2& u) { return { num + u.x, num + u.y }; }
	friend Vector2 operator+(const Vector2& u, const Vector2& v) { return { u.x + v.x, u.y + v.y }; }

	Vector2 operator-() const { return { -x, -y }; }
	friend Vector2 operator-(const Vector2& u, float num) { return { u.x - num, u.y - num }; }
	friend Vector2 operator-(float num, const Vector2& u) { return { num - u.x, num - u.y }; }
	friend Vector2 operator-(const Vector2& u, const Vector2& v) { return { u.x - v.x, u.y - v.y }; }

	friend Vector2 operator*(const Vector2& u, float num) { return { u.x * num, u.y * num }; }
	friend Vector2 operator*(float num, const Vector2& u) { return { num * u.x, num * u.y }; }
	friend Vector2 operator*(const Vector2& u, const Vector2& v) { return { u.x * v.x, u.y * v.y }; }

	friend Vector2 operator/(const Vector2& u, float num) { return { u.x / num, u.y / num }; }
	friend Vector2 operator/(float num, const Vector2& u) { return { num / u.x, num / u.y }; }
	friend Vector2 operator/(const Vector2& u, const Vector2& v) { return { u.x / v.x, u.y / v.y }; }

	Vector2& operator+=(float num) { x += num; y += num; return *this; }
	Vector2& operator+=(const Vector2& u) { x += u.x; y += u.y; return *this; }
	Vector2& operator-=(float num) { x -= num; y -= num; return *this; }
	Vector2& operator-=(const Vector2& u) { x -= u.x; y -= u.y; return *this; }
	Vector2& operator*=(float num) { x *= num; y *= num; return *this; }
	Vector2& operator*=(const Vector2& u) { x *= u.x; y *= u.y; return *this; }
	Vector2& operator/=(float num) { x /= num; y /= num; return *this; }
	Vector2& operator/=(const Vector2& u) { x /= u.x; y /= u.y; return *this; }

	constexpr float& operator[](size_t i) { return (&x)[i]; }
	constexpr const float operator[](size_t i) const { return (&x)[i]; }

	[[nodiscard]] float GetLength() const;
	[[nodiscard]] float GetLengthSquared() const;
	[[nodiscard]] Vector2 Normalize() const;

	float x = 0.0f;
	float y = 0.0f;
};

inline float Vector2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

inline float Vector2::GetLengthSquared() const
{
	return x * x + y * y;
}

inline Vector2 Vector2::Normalize() const
{
	const float invLen = 1.0f / sqrtf(x * x + y * y);
	return { x * invLen, y * invLen };
}

inline float Distance(const Vector2& v1, const Vector2& v2)
{
	return (v1 - v2).GetLength();
}

inline float DotProduct(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline Vector2 Mix(const Vector2& u, const Vector2& v, float a)
{
	return u * (1.0f - a) + v * a;
}

//=============================================================================
// Vector3
//=============================================================================
class Vector3
{
public:
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Right;
	static const Vector3 Up;
	static const Vector3 Forward;

	constexpr Vector3() = default;
	constexpr Vector3(Vector3&&) = default;
	constexpr Vector3(const Vector3&) = default;
	constexpr Vector3(float n) : x(n), y(n), z(n) {}
	constexpr Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	constexpr Vector3& operator=(Vector3&&) = default;
	constexpr Vector3& operator=(const Vector3&) = default;

	friend Vector3 operator+(const Vector3& u, float num) { return { u.x + num, u.y + num, u.z + num }; }
	friend Vector3 operator+(float num, const Vector3& u) { return { num + u.x, num + u.y, num + u.z }; }
	friend Vector3 operator+(const Vector3& u, const Vector3& v) { return { u.x + v.x, u.y + v.y, u.z + v.z }; }
	constexpr Vector3 operator-() const { return { -x, -y, -z }; }
	friend Vector3 operator-(const Vector3& u, float num) { return { u.x - num, u.y - num, u.z - num }; }
	friend Vector3 operator-(float num, const Vector3& u) { return { num - u.x, num - u.y, num - u.z }; }
	friend Vector3 operator-(const Vector3& u, const Vector3& v) { return { u.x - v.x, u.y - v.y, u.z - v.z }; }
	friend Vector3 operator*(const Vector3& u, float num) { return { u.x * num, u.y * num, u.z * num }; }
	friend Vector3 operator*(float num, const Vector3& u) { return { num * u.x, num * u.y, num * u.z }; }
	friend Vector3 operator*(const Vector3& u, const Vector3& v) { return { u.x * v.x, u.y * v.y, u.z * v.z }; }
	friend Vector3 operator/(const Vector3& u, float num) { return { u.x / num, u.y / num, u.z / num }; }
	friend Vector3 operator/(float num, const Vector3& u) { return { num / u.x, num / u.y, num / u.z }; }
	friend Vector3 operator/(const Vector3& u, const Vector3& v) { return { u.x / v.x, u.y / v.y, u.z / v.z }; }

	Vector3& operator+=(float num) { x += num; y += num; z += num; return *this; }
	Vector3& operator+=(const Vector3& u) { x += u.x; y += u.y; z += u.z; return *this; }
	Vector3& operator-=(float num) { x -= num; y -= num; z -= num; return *this; }
	Vector3& operator-=(const Vector3& u) { x -= u.x; y -= u.y; z -= u.z; return *this; }
	Vector3& operator*=(float num) { x *= num; y *= num; z *= num; return *this; }
	Vector3& operator*=(const Vector3& u) { x *= u.x; y *= u.y; z *= u.z; return *this; }
	Vector3& operator/=(float num) { x /= num; y /= num; z /= num; return *this; }
	Vector3& operator/=(const Vector3& u) { x /= u.x; y /= u.y; z /= u.z; return *this; }

	constexpr float& operator[](size_t i) { return (&x)[i]; }
	constexpr const float operator[](size_t i) const { return (&x)[i]; }

	[[nodiscard]] float GetLength() const;
	[[nodiscard]] float GetLengthSquared() const;
	[[nodiscard]] Vector3 Normalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline const Vector3 Vector3::Zero = { 0.0f };
inline const Vector3 Vector3::One = { 1.0f };
inline const Vector3 Vector3::Right = { 1.0f, 0.0f, 0.0f };
inline const Vector3 Vector3::Up = { 0.0f, 1.0f, 0.0f };
inline const Vector3 Vector3::Forward = { 0.0f, 0.0f, 1.0f };

inline float Vector3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

inline float Vector3::GetLengthSquared() const
{
	return x * x + y * y + z * z;
}

inline Vector3 Vector3::Normalize() const
{
	const float invLen = 1.0f / sqrtf(x * x + y * y + z * z);
	return { x * invLen, y * invLen, z * invLen };
}

inline float Distance(const Vector3& v1, const Vector3& v2)
{
	return (v1 - v2).GetLength();
}

inline float DotProduct(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
{
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

inline Vector3 Min(const Vector3& v1, const Vector3& v2)
{
	return {
		Min(v1.x, v2.x),
		Min(v1.y, v2.y),
		Min(v1.z, v2.z),
	};
}
inline Vector3 Max(const Vector3& v1, const Vector3& v2)
{
	return {
		Max(v1.x, v2.x),
		Max(v1.y, v2.y),
		Max(v1.z, v2.z),
	};
}

inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x)
{
	return a + (b - a) * x;
}

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