#pragma once

//=============================================================================
// Рандом заметки
//=============================================================================
/*
DotProduct - скалярное произведение векторов
	
	DotProduct(a,b) = DotProduct(b,a)

		> 0 - вектора направлены в одну сторону
		= 0 - вектора перпендикулярны (90 градусов)
		< 0 - вектора направлены противоположно
		= 1 - паралельны?
		= 0.7 - 45 градусов?
			пример - Dot(EnemyPos - PlayerPos, Forward) > 0 - враг перед нами
			или освещение по ламберту - max(dot(Light.Direction, NormalVector)), 0.0);
				идея такая - если свет светит прямо на поверхность то вектор направления света паралелен нормали поверхности - dot вернет 1.
	DotProduct(na,nb) = cos(angle_rad_to_ab) - скалярное произведение нормализованных векторов равно косинусу угла между ними

CrossProduct - векторное произведение векторов.
	Может быть только в 3д
	Результат - получается вектор перпендикулярный двум другим. Пример - оси xyz - векторным произведением векторов x и y будет вектор z
	
	CrossProduct(a,b) != CrossProduct(b,a)

	Примеры применения
		площадь треугольника - float area = Length(Cross(u,v)) / 2;
		Нормаль треугольника - vec3 normal = Normalize(Cross(C-A, B-A));

		Коррекция движения при коллизиях (скольжение вдоль стены)
			dir - тек вектор движения
			normal - нормаль поверхности
			vec3 tempVec = cross(dir, normal);
			vec3 newDir = cross(tempVec, normal);

			(но можно еще быстрее через нахождение отраженного вектора
				vec3 newDir = dir - normal*dir(dir, normal);

*/



//=============================================================================
// Header
//=============================================================================
#include <math.h>

//=============================================================================
// Core
//=============================================================================
constexpr const float EPSILON = 1e-6f;
constexpr const float PI = 3.14159265358979323846f;
constexpr const float HALF_PI = PI * 0.5f;
constexpr const float DEG2RAD = PI / 180.0f;
constexpr const float RAD2DEG = 180.0f / PI;

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

inline constexpr float Lerp(float lower, float upper, float gradient) noexcept
{
	return lower + (upper - lower) * Max(0.0f, Min(gradient, 1.0f));
}

inline bool Compare(float a, float b, float eps = EPSILON) { return fabsf(a - b) < eps; }

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
	constexpr Color(unsigned uintRGBA) 
	{
		a = (unsigned char)(uintRGBA >> 24);
		r = (unsigned char)(uintRGBA >> 16);
		g = (unsigned char)(uintRGBA >> 8);
		b = (unsigned char)(uintRGBA >> 0);
	}
	constexpr Color(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na = 255) : r(nr), g(ng), b(nb), a(na) {}

	constexpr Color& operator=(Color&&) = default;
	constexpr Color& operator=(const Color&) = default;

	unsigned ToUInt() const { return (unsigned)a << 24 | r << 16 | g << 8 | b; }

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

	Vector2 operator-() const { return { -x, -y }; }

	friend Vector2 operator-(const Vector2& u, float num) { return { u.x - num, u.y - num }; }
	friend Vector2 operator-(float num, const Vector2& u) { return { num - u.x, num - u.y }; }
	friend Vector2 operator-(const Vector2& u, const Vector2& v) { return { u.x - v.x, u.y - v.y }; }
	friend Vector2 operator+(const Vector2& u, float num) { return { u.x + num, u.y + num }; }
	friend Vector2 operator+(float num, const Vector2& u) { return { num + u.x, num + u.y }; }
	friend Vector2 operator+(const Vector2& u, const Vector2& v) { return { u.x + v.x, u.y + v.y }; }
	friend Vector2 operator*(const Vector2& u, float num) { return { u.x * num, u.y * num }; }
	friend Vector2 operator*(float num, const Vector2& u) { return { num * u.x, num * u.y }; }
	friend Vector2 operator*(const Vector2& u, const Vector2& v) { return { u.x * v.x, u.y * v.y }; }
	friend Vector2 operator/(const Vector2& u, float num) { return { u.x / num, u.y / num }; }
	friend Vector2 operator/(float num, const Vector2& u) { return { num / u.x, num / u.y }; }
	friend Vector2 operator/(const Vector2& u, const Vector2& v) { return { u.x / v.x, u.y / v.y }; }

	Vector2& operator-=(float num) { x -= num; y -= num; return *this; }
	Vector2& operator-=(const Vector2& u) { x -= u.x; y -= u.y; return *this; }
	Vector2& operator+=(float num) { x += num; y += num; return *this; }
	Vector2& operator+=(const Vector2& u) { x += u.x; y += u.y; return *this; }
	Vector2& operator*=(float num) { x *= num; y *= num; return *this; }
	Vector2& operator*=(const Vector2& u) { x *= u.x; y *= u.y; return *this; }
	Vector2& operator/=(float num) { x /= num; y /= num; return *this; }
	Vector2& operator/=(const Vector2& u) { x /= u.x; y /= u.y; return *this; }

	float& operator[](size_t i) { return (&x)[i]; }
	const float operator[](size_t i) const { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector2 Normalize() const;

	float x = 0.0f;
	float y = 0.0f;
};

inline float Distance(const Vector2& v1, const Vector2& v2);
inline float DotProduct(const Vector2& v1, const Vector2& v2);
inline Vector2 Min(const Vector2& v1, const Vector2& v2);
inline Vector2 Max(const Vector2& v1, const Vector2& v2);
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x);
inline Vector2 Mix(const Vector2& u, const Vector2& v, float a);

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

	Vector3 operator-() const { return { -x, -y, -z }; }
	friend Vector3 operator-(const Vector3& u, float num) { return { u.x - num, u.y - num, u.z - num }; }
	friend Vector3 operator-(float num, const Vector3& u) { return { num - u.x, num - u.y, num - u.z }; }
	friend Vector3 operator-(const Vector3& u, const Vector3& v) { return { u.x - v.x, u.y - v.y, u.z - v.z }; }
	friend Vector3 operator+(const Vector3& u, float num) { return { u.x + num, u.y + num, u.z + num }; }
	friend Vector3 operator+(float num, const Vector3& u) { return { num + u.x, num + u.y, num + u.z }; }
	friend Vector3 operator+(const Vector3& u, const Vector3& v) { return { u.x + v.x, u.y + v.y, u.z + v.z }; }
	friend Vector3 operator*(const Vector3& u, float num) { return { u.x * num, u.y * num, u.z * num }; }
	friend Vector3 operator*(float num, const Vector3& u) { return { num * u.x, num * u.y, num * u.z }; }
	friend Vector3 operator*(const Vector3& u, const Vector3& v) { return { u.x * v.x, u.y * v.y, u.z * v.z }; }
	friend Vector3 operator/(const Vector3& u, float num) { return { u.x / num, u.y / num, u.z / num }; }
	friend Vector3 operator/(float num, const Vector3& u) { return { num / u.x, num / u.y, num / u.z }; }
	friend Vector3 operator/(const Vector3& u, const Vector3& v) { return { u.x / v.x, u.y / v.y, u.z / v.z }; }

	Vector3& operator-=(float num) { x -= num; y -= num; z -= num; return *this; }
	Vector3& operator-=(const Vector3& u) { x -= u.x; y -= u.y; z -= u.z; return *this; }
	Vector3& operator+=(float num) { x += num; y += num; z += num; return *this; }
	Vector3& operator+=(const Vector3& u) { x += u.x; y += u.y; z += u.z; return *this; }
	Vector3& operator*=(float num) { x *= num; y *= num; z *= num; return *this; }
	Vector3& operator*=(const Vector3& u) { x *= u.x; y *= u.y; z *= u.z; return *this; }
	Vector3& operator/=(float num) { x /= num; y /= num; z /= num; return *this; }
	Vector3& operator/=(const Vector3& u) { x /= u.x; y /= u.y; z /= u.z; return *this; }

	float& operator[](size_t i) { return (&x)[i]; }
	const float operator[](size_t i) const { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector3 Normalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline float Distance(const Vector3& v1, const Vector3& v2);
inline float DotProduct(const Vector3& v1, const Vector3& v2);
inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);
inline Vector3 Min(const Vector3& v1, const Vector3& v2);
inline Vector3 Max(const Vector3& v1, const Vector3& v2);
inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x);
inline Vector3 Mix(const Vector3& u, const Vector3& v, float t);
inline Vector3 Rotate(const Vector3& u, float angle, const Vector3& v);

//=============================================================================
// Vector4
//=============================================================================
class Vector4
{
public:
	constexpr Vector4() = default;
	constexpr Vector4(Vector4&&) = default;
	constexpr Vector4(const Vector4&) = default;
	constexpr Vector4(float n) : x(n), y(n), z(n), w(n) {}
	constexpr Vector4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}

	constexpr Vector4& operator=(Vector4&&) = default;
	constexpr Vector4& operator=(const Vector4&) = default;

	Vector4 operator-() const { return { -x, -y, -z, -w }; }
	friend Vector4 operator-(const Vector4& u, float num) { return { u.x - num, u.y - num, u.z - num, u.w - num }; }
	friend Vector4 operator-(float num, const Vector4& u) { return { num - u.x, num - u.y, num - u.z, num - u.w }; }
	friend Vector4 operator-(const Vector4& u, const Vector4& v) { return { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w }; }
	friend Vector4 operator+(const Vector4& u, float num) { return { u.x + num, u.y + num, u.z + num, u.w + num }; }
	friend Vector4 operator+(float num, const Vector4& u) { return { num + u.x, num + u.y, num + u.z, num + u.w }; }
	friend Vector4 operator+(const Vector4& u, const Vector4& v) { return { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w }; }
	friend Vector4 operator*(const Vector4& u, float num) { return { u.x * num, u.y * num, u.z * num, u.w * num }; }
	friend Vector4 operator*(float num, const Vector4& u) { return { num * u.x, num * u.y, num * u.z, num * u.w }; }
	friend Vector4 operator*(const Vector4& u, const Vector4& v) { return { u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w }; }
	friend Vector4 operator/(const Vector4& u, float num) { return { u.x / num, u.y / num, u.z / num, u.w / num }; }
	friend Vector4 operator/(float num, const Vector4& u) { return { num / u.x, num / u.y, num / u.z, num / u.w }; }
	friend Vector4 operator/(const Vector4& u, const Vector4& v) { return { u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w }; }

	Vector4& operator-=(float num) { x -= num; y -= num; z -= num; w -= num; return *this; }
	Vector4& operator-=(const Vector4& u) { x -= u.x; y -= u.y; z -= u.z; w -= u.w; return *this; }
	Vector4& operator+=(float num) { x += num; y += num; z += num; w += num; return *this; }
	Vector4& operator+=(const Vector4& u) { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }
	Vector4& operator*=(float num) { x *= num; y *= num; z *= num; w *= num; return *this; }
	Vector4& operator*=(const Vector4& u) { x *= u.x; y *= u.y; z *= u.z; w *= u.w; return *this; }
	Vector4& operator/=(float num) { x /= num; y /= num; z /= num; w /= num; return *this; }
	Vector4& operator/=(const Vector4& u) { x /= u.x; y /= u.y; z /= u.z; w /= u.w; return *this; }

	float& operator[](size_t i) { return (&x)[i]; }
	const float operator[](size_t i) const { return (&x)[i]; }

	float* operator&() { return (float*)this; }

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

//=============================================================================
// Quat
//=============================================================================

class Quat
{
public:
	constexpr Quat() = default;
	constexpr Quat(Quat&&) = default;
	constexpr Quat(const Quat&) = default;
	constexpr Quat(float nx, float ny, float nz, float nw = 1.0f) : x(nx), y(ny), z(nz), w(nw) {}
	Quat(const Vector3& axis, float angle);

	constexpr Quat& operator=(Quat&&) = default;
	constexpr Quat& operator=(const Quat&) = default;

	constexpr Quat operator*(const Quat& q) const;
	constexpr Quat operator-() const { return { x, y, z, -w }; }
	constexpr Quat operator+(const Quat& q) const { return { x + q.x, y + q.y, z + q.z, w + q.w }; }
	constexpr Quat operator*(float m) const { return { x * m, y * m, z * m, w * m }; }
	Vector3 operator*(const Vector3& q) const { return Rotate(q); }

	constexpr void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }

	void FromEuler(const Vector3& euler);
	[[nodiscard]] Vector3 ToEuler() const;

	[[nodiscard]] Vector3 Rotate(const Vector3& v) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

//=============================================================================
// Matrix3
//=============================================================================

class Matrix4;

class Matrix3
{
public:
	constexpr Matrix3() = default;
	constexpr Matrix3(Matrix3&&) = default;
	constexpr Matrix3(const Matrix3&) = default;
	constexpr Matrix3(const float* f);
	constexpr Matrix3(const Vector3 &col1, const Vector3 &col2, const Vector3 &col3);
	constexpr Matrix3(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);
	Matrix3(const Matrix4& m);

	constexpr Matrix3& operator=(Matrix3&&) = default;
	constexpr Matrix3& operator=(const Matrix3&) = default;

	constexpr float& operator[](size_t i) { return m[i]; }
	constexpr const float operator[](size_t i) const { return m[i]; }

	friend Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2);
	friend Vector3 operator*(const Matrix3 &m, const Vector3 &u);

	constexpr void Set(const float* f);
	constexpr void Set(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);
	constexpr void Set(const Matrix3& M);

	[[nodiscard]] Matrix3 Inverse() const;
	[[nodiscard]] Matrix3 Transpose() const;

	float m[9] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
};

//=============================================================================
// Matrix4
//=============================================================================

class Matrix4
{
public:
	constexpr Matrix4() = default;
	constexpr Matrix4(Matrix4&&) = default;
	constexpr Matrix4(const Matrix4&) = default;
	constexpr Matrix4(const float* f);
	constexpr Matrix4(const Vector4 &col1, const Vector4 &col2, const Vector4 &col3, const Vector4 &col4);
	constexpr Matrix4(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15);
	constexpr explicit Matrix4(const Matrix3 &mat);

	constexpr Matrix4& operator=(Matrix4&&) = default;
	constexpr Matrix4& operator=(const Matrix4&) = default;

	constexpr float& operator[](size_t i) { return m[i]; }
	constexpr const float operator[](size_t i) const { return m[i]; }

	friend Matrix4 operator*(const Matrix4 &Matrix1, const Matrix4 &Matrix2);
	friend Vector4 operator*(const Matrix4 &m, const Vector4 &u);

	constexpr void Set(const float* f);
	constexpr void Set(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15);
	constexpr void Set(const Matrix4& M);

	[[nodiscard]] Matrix4 Inverse() const;
	[[nodiscard]] Matrix4 Transpose() const;

	void Decompose(Vector3& outScale, Quat& outRotation, Vector3& outTranslation) const;

	[[nodiscard]] static Matrix4 Rotate(const Vector3& axis, float angle);
	[[nodiscard]] static Matrix4 RotateX(float angle);
	[[nodiscard]] static Matrix4 RotateY(float angle);
	[[nodiscard]] static Matrix4 RotateZ(float angle);
	[[nodiscard]] static Matrix4 Scale(const Vector3& v);
	[[nodiscard]] static Matrix4 Translate(const Vector3& v);

	[[nodiscard]] static Matrix4 Ortho(float left, float right, float bottom, float top, float n, float f);
	[[nodiscard]] static Matrix4 Perspective(float fieldOfView, float aspectRatio, float znear, float zfar);
	[[nodiscard]] static Matrix4 LookAt(const Vector3& eye, const Vector3& dir, const Vector3& up);

	float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
};

//=============================================================================
// Vector2 Impl
//=============================================================================

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

inline Vector2 Min(const Vector2& v1, const Vector2& v2)
{
	return {
		Min(v1.x, v2.x),
		Min(v1.y, v2.y),
	};
}

inline Vector2 Max(const Vector2& v1, const Vector2& v2)
{
	return {
		Max(v1.x, v2.x),
		Max(v1.y, v2.y),
	};
}

inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x)
{
	return a + (b - a) * x;
}

inline Vector2 Mix(const Vector2& u, const Vector2& v, float a)
{
	return u * (1.0f - a) + v * a;
}

//=============================================================================
// Vector3 Impl
//=============================================================================

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

inline Vector3 Mix(const Vector3& u, const Vector3& v, float t)
{
	return u * (1.0f - t) + v * t;
}

inline Vector3 Rotate(const Vector3& u, float angle, const Vector3& v)
{
	return *(Vector3*)&(Matrix4::Rotate(v, angle) * Vector4(u.x, u.y, u.z, 1.0f));
}

//=============================================================================
// Quat Impl
//=============================================================================

inline Quat::Quat(const Vector3& axis, float angle)
{
	const float half_angle = angle * 0.5f;
	const float s = sinf(half_angle);
	w = cosf(half_angle);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}

inline constexpr Quat Quat::operator*(const Quat& rhs) const
{
	return {
		w * rhs.x + rhs.w * x + y * rhs.z - rhs.y * z,
		w * rhs.y + rhs.w * y + z * rhs.x - rhs.z * x,
		w * rhs.z + rhs.w * z + x * rhs.y - rhs.x * y,
		w * rhs.w - rhs.x * x - y * rhs.y - rhs.z * z };
}

inline void Quat::FromEuler(const Vector3& euler)
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

	w = cosY * cosX * cosZ + sinY * sinX * sinZ;
	x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

inline Vector3 Quat::ToEuler() const
{
	const float check = 2.0f * (w * x - y * z);

	if( check < -0.999999f )
	{
		return {
			-HALF_PI,
			0.0f,
			-atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
		};
	}

	if( check > 0.999999f )
	{
		return {
			HALF_PI,
			0.0f,
			atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
		};
	}

	return {
		(float)asin(check),
		atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)),
		atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z))
	};
}

inline Vector3 Quat::Rotate(const Vector3& v) const
{
	const Vector3 qvec(x, y, z);
	Vector3 uv = CrossProduct(qvec, v);
	Vector3 uuv = CrossProduct(qvec, uv);
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return v + uv + uuv;
}

//=============================================================================
// Matrix3 Impl
//=============================================================================

inline constexpr Matrix3::Matrix3(const float* f)
{
	m[0] = f[0]; m[1] = f[1]; m[2] = f[2];
	m[3] = f[3]; m[4] = f[4]; m[5] = f[5];
	m[6] = f[6]; m[7] = f[7]; m[8] = f[8];
}

inline constexpr Matrix3::Matrix3(const Vector3 & col1, const Vector3 & col2, const Vector3 & col3)
{
	m[0] = col1.x; m[3] = col2.x; m[6] = col3.x;
	m[1] = col1.y; m[4] = col2.y; m[7] = col3.y;
	m[2] = col1.z; m[5] = col2.z; m[8] = col3.z;
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

inline Matrix3::Matrix3(const Matrix4& M)
{
	m[0] = M[0]; m[3] = M[4]; m[6] = M[8];
	m[1] = M[1]; m[4] = M[5]; m[7] = M[9];
	m[2] = M[2]; m[5] = M[6]; m[8] = M[10];
}

inline Matrix3 operator*(const Matrix3& m1, const Matrix3& m2)
{
	return {
		m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2],
		m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2],
		m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2],
		m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5],
		m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5],
		m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5],
		m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8],
		m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8],
		m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8],
	};
}

inline Vector3 operator*(const Matrix3 &m, const Vector3 &u)
{
	return {
		m[0] * u.x + m[3] * u.y + m[6] * u.z,
		m[1] * u.x + m[4] * u.y + m[7] * u.z,
		m[2] * u.x + m[5] * u.y + m[8] * u.z
	};
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

inline float det2x2sub(const float *m, int i0, int i1, int i2, int i3)
{
	return m[i0] * m[i3] - m[i2] * m[i1];
}

inline Matrix3 Matrix3::Inverse() const
{
	float det = 0.0f;
	det += m[0] * det2x2sub(m, 4, 5, 7, 8);
	det -= m[3] * det2x2sub(m, 1, 2, 7, 8);
	det += m[6] * det2x2sub(m, 1, 2, 4, 5);

	Matrix3 Inverse;
	Inverse[0] = det2x2sub(m, 4, 5, 7, 8) / det;
	Inverse[1] = -det2x2sub(m, 1, 2, 7, 8) / det;
	Inverse[2] = det2x2sub(m, 1, 2, 4, 5) / det;
	Inverse[3] = -det2x2sub(m, 3, 5, 6, 8) / det;
	Inverse[4] = det2x2sub(m, 0, 2, 6, 8) / det;
	Inverse[5] = -det2x2sub(m, 0, 2, 3, 5) / det;
	Inverse[6] = det2x2sub(m, 3, 4, 6, 7) / det;
	Inverse[7] = -det2x2sub(m, 0, 1, 6, 7) / det;
	Inverse[8] = det2x2sub(m, 0, 1, 3, 4) / det;
	return Inverse;
}

inline Matrix3 Matrix3::Transpose() const
{
	Matrix3 Transpose;
	Transpose[0] = m[0];
	Transpose[1] = m[3];
	Transpose[2] = m[6];
	Transpose[3] = m[1];
	Transpose[4] = m[4];
	Transpose[5] = m[7];
	Transpose[6] = m[2];
	Transpose[7] = m[5];
	Transpose[8] = m[8];
	return Transpose;
}

//=============================================================================
// Matrix4 Impl
//=============================================================================

inline constexpr Matrix4::Matrix4(const float* f)
{
	m[0] = f[0];  m[1] = f[1];  m[2] = f[2];  m[3] = f[3];
	m[4] = f[4];  m[5] = f[5];  m[6] = f[6];  m[7] = f[7];
	m[8] = f[8];  m[9] = f[9];  m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr Matrix4::Matrix4(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4)
{
	m[0] = col1.x; m[4] = col2.x; m[ 8] = col3.x; m[12] = col4.x;
	m[1] = col1.y; m[5] = col2.y; m[ 9] = col3.y; m[13] = col4.y;
	m[2] = col1.z; m[6] = col2.z; m[10] = col3.z; m[14] = col4.z;
	m[3] = col1.w; m[7] = col2.w; m[11] = col3.w; m[15] = col4.w;
}

inline constexpr Matrix4::Matrix4(
	float  m0, float  m1, float  m2, float  m3,
	float  m4, float  m5, float  m6, float  m7,
	float  m8, float  m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
	m[4] = m4;  m[5] = m5;  m[6] = m6;  m[7] = m7;
	m[8] = m8;  m[9] = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

inline constexpr Matrix4::Matrix4(const Matrix3& mat)
{
	m[0] = mat[0]; m[4] = mat[3]; m[ 8] = mat[6]; m[12] = 0.0f;
	m[1] = mat[1]; m[5] = mat[4]; m[ 9] = mat[7]; m[13] = 0.0f;
	m[2] = mat[2]; m[6] = mat[5]; m[10] = mat[8]; m[14] = 0.0f;
	m[3] = 0.0f;   m[7] = 0.0f;   m[11] = 0.0f;   m[15] = 1.0f;
}

inline Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	return {
		m1[0] * m2[ 0] + m1[4] * m2[ 1] + m1[ 8] * m2[ 2] + m1[12] * m2[ 3],
		m1[1] * m2[ 0] + m1[5] * m2[ 1] + m1[ 9] * m2[ 2] + m1[13] * m2[ 3],
		m1[2] * m2[ 0] + m1[6] * m2[ 1] + m1[10] * m2[ 2] + m1[14] * m2[ 3],
		m1[3] * m2[ 0] + m1[7] * m2[ 1] + m1[11] * m2[ 2] + m1[15] * m2[ 3],
		m1[0] * m2[ 4] + m1[4] * m2[ 5] + m1[ 8] * m2[ 6] + m1[12] * m2[ 7],
		m1[1] * m2[ 4] + m1[5] * m2[ 5] + m1[ 9] * m2[ 6] + m1[13] * m2[ 7],
		m1[2] * m2[ 4] + m1[6] * m2[ 5] + m1[10] * m2[ 6] + m1[14] * m2[ 7],
		m1[3] * m2[ 4] + m1[7] * m2[ 5] + m1[11] * m2[ 6] + m1[15] * m2[ 7],
		m1[0] * m2[ 8] + m1[4] * m2[ 9] + m1[ 8] * m2[10] + m1[12] * m2[11],
		m1[1] * m2[ 8] + m1[5] * m2[ 9] + m1[ 9] * m2[10] + m1[13] * m2[11],
		m1[2] * m2[ 8] + m1[6] * m2[ 9] + m1[10] * m2[10] + m1[14] * m2[11],
		m1[3] * m2[ 8] + m1[7] * m2[ 9] + m1[11] * m2[10] + m1[15] * m2[11],
		m1[0] * m2[12] + m1[4] * m2[13] + m1[ 8] * m2[14] + m1[12] * m2[15],
		m1[1] * m2[12] + m1[5] * m2[13] + m1[ 9] * m2[14] + m1[13] * m2[15],
		m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15],
		m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15],
	};
}

inline Vector4 operator*(const Matrix4& m, const Vector4& u)
{
	return {
		m[0] * u.x + m[4] * u.y + m[ 8] * u.z + m[12] * u.w,
		m[1] * u.x + m[5] * u.y + m[ 9] * u.z + m[13] * u.w,
		m[2] * u.x + m[6] * u.y + m[10] * u.z + m[14] * u.w,
		m[3] * u.x + m[7] * u.y + m[11] * u.z + m[15] * u.w,
	};
}

inline constexpr void Matrix4::Set(const float* f)
{
	m[0] = f[0];  m[1] = f[1];  m[2] = f[2];  m[3] = f[3];
	m[4] = f[4];  m[5] = f[5];  m[6] = f[6];  m[7] = f[7];
	m[8] = f[8];  m[9] = f[9];  m[10] = f[10]; m[11] = f[11];
	m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
}

inline constexpr void Matrix4::Set(
	float  m0, float  m1, float  m2, float  m3,
	float  m4, float  m5, float  m6, float  m7,
	float  m8, float  m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
	m[4] = m4;  m[5] = m5;  m[6] = m6;  m[7] = m7;
	m[8] = m8;  m[9] = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

inline constexpr void Matrix4::Set(const Matrix4& M)
{
	m[0] = M[0];  m[1] = M[1];   m[2] = M[2];  m[3] = M[3];
	m[4] = M[4];  m[5] = M[5];   m[6] = M[6];  m[7] = M[7];
	m[8] = M[8];  m[9] = M[9];   m[10] = M[10]; m[11] = M[11];
	m[12] = M[12]; m[13] = M[13]; m[14] = M[14]; m[15] = M[15];
}

inline float det3x3sub(const float *m, int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
	float det = 0.0f;
	det += m[i0] * det2x2sub(m, i4, i5, i7, i8);
	det -= m[i3] * det2x2sub(m, i1, i2, i7, i8);
	det += m[i6] * det2x2sub(m, i1, i2, i4, i5);
	return det;
}

inline Matrix4 Matrix4::Inverse() const
{
	float det = 0.0f;
	det += m[0] * det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15);
	det -= m[4] * det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	det += m[8] * det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	det -= m[12] * det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11);

	Matrix4 Inverse;
	Inverse[ 0] = det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15) / det;
	Inverse[ 1] = -det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15) / det;
	Inverse[ 2] = det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15) / det;
	Inverse[ 3] = -det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11) / det;
	Inverse[ 4] = -det3x3sub(m, 4, 6, 7, 8, 10, 11, 12, 14, 15) / det;
	Inverse[ 5] = det3x3sub(m, 0, 2, 3, 8, 10, 11, 12, 14, 15) / det;
	Inverse[ 6] = -det3x3sub(m, 0, 2, 3, 4, 6, 7, 12, 14, 15) / det;
	Inverse[ 7] = det3x3sub(m, 0, 2, 3, 4, 6, 7, 8, 10, 11) / det;
	Inverse[ 8] = det3x3sub(m, 4, 5, 7, 8, 9, 11, 12, 13, 15) / det;
	Inverse[ 9] = -det3x3sub(m, 0, 1, 3, 8, 9, 11, 12, 13, 15) / det;
	Inverse[10] = det3x3sub(m, 0, 1, 3, 4, 5, 7, 12, 13, 15) / det;
	Inverse[11] = -det3x3sub(m, 0, 1, 3, 4, 5, 7, 8, 9, 11) / det;
	Inverse[12] = -det3x3sub(m, 4, 5, 6, 8, 9, 10, 12, 13, 14) / det;
	Inverse[13] = det3x3sub(m, 0, 1, 2, 8, 9, 10, 12, 13, 14) / det;
	Inverse[14] = -det3x3sub(m, 0, 1, 2, 4, 5, 6, 12, 13, 14) / det;
	Inverse[15] = det3x3sub(m, 0, 1, 2, 4, 5, 6, 8, 9, 10) / det;
	return Inverse;
}

inline Matrix4 Matrix4::Transpose() const
{
	Matrix4 Transpose;
	Transpose[ 0] = m[0];
	Transpose[ 1] = m[4];
	Transpose[ 2] = m[8];
	Transpose[ 3] = m[12];
	Transpose[ 4] = m[1];
	Transpose[ 5] = m[5];
	Transpose[ 6] = m[9];
	Transpose[ 7] = m[13];
	Transpose[ 8] = m[2];
	Transpose[ 9] = m[6];
	Transpose[10] = m[10];
	Transpose[11] = m[14];
	Transpose[12] = m[3];
	Transpose[13] = m[7];
	Transpose[14] = m[11];
	Transpose[15] = m[15];
	return Transpose;
}

inline Matrix4 Matrix4::Rotate(const Vector3& axis, float angle)
{
	const float s = sinf(angle);
	const float c = 1.0f - cosf(angle);
	Vector3 v = axis.Normalize();
	const float xx = v.x * v.x;
	const float xy = v.x * v.y;
	const float xz = v.z * v.x;
	const float yy = v.y * v.y;
	const float yz = v.y * v.z;
	const float zz = v.z * v.z;
	const float xs = v.x * s;
	const float ys = v.y * s;
	const float zs = v.z * s;
	Matrix4 mat;
	mat[0] = 1.0f + c * (xx - 1.0f); mat[4] = c * xy - zs;            mat[ 8] = c * xz + ys;            mat[12] = 0.0f;
	mat[1] = c * xy + zs;            mat[5] = 1.0f + c * (yy - 1.0f); mat[ 9] = c * yz - xs;            mat[13] = 0.0f;
	mat[2] = c * xz - ys;            mat[6] = c * yz + xs;            mat[10] = 1.0f + c * (zz - 1.0f); mat[14] = 0.0f;
	mat[3] = 0.0f;                   mat[7] = 0.0f;                   mat[11] = 0.0f;                   mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4::RotateX(float angle)
{
	const float s = sinf(angle);
	const float c = cosf(angle);

	Matrix4 mat;
	mat[0] = 1.0f; mat[4] = 0.0f; mat[ 8] = 0.0f; mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = c;    mat[ 9] = -s;   mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = s;    mat[10] = c;    mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4::RotateY(float angle)
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

inline Matrix4 Matrix4::RotateZ(float angle)
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

inline Matrix4 Matrix4::Scale(const Vector3& v)
{
	Matrix4 mat;
	mat[0] = v.x;  mat[4] = 0.0f; mat[ 8] = 0.0f; mat[12] = 0.0f;
	mat[1] = 0.0f; mat[5] = v.y;  mat[ 9] = 0.0f; mat[13] = 0.0f;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = v.z;  mat[14] = 0.0f;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4::Translate(const Vector3& v)
{
	Matrix4 mat;
	mat[0] = 1.0f; mat[4] = 0.0f; mat[ 8] = 0.0f; mat[12] = v.x;
	mat[1] = 0.0f; mat[5] = 1.0f; mat[ 9] = 0.0f; mat[13] = v.y;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = 1.0f; mat[14] = v.z;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float n, float f)
{
	Matrix4 mat;
	mat[0] = 2.0f / (right - left); mat[4] = 0.0f;                  mat[ 8] = 0.0f;            mat[12] = -(right + left) / (right - left);
	mat[1] = 0.0f;                  mat[5] = 2.0f / (top - bottom); mat[ 9] = 0.0f;            mat[13] = -(top + bottom) / (top - bottom);
	mat[2] = 0.0f;                  mat[6] = 0.0f;                  mat[10] = -2.0f / (f - n); mat[14] = -(f + n) / (f - n);
	mat[3] = 0.0f;                  mat[7] = 0.0f;                  mat[11] = 0.0f;            mat[15] = 1.0f;
	return mat;
}

inline Matrix4 Matrix4::Perspective(float fieldOfView, float aspectRatio, float znear, float zfar)
{
	const float y = tanf(fieldOfView * 0.5f);
	const float x = y * aspectRatio;
	Matrix4 mat;
	mat[0] = 1.0f / x; mat[4] = 0.0f;     mat[8] = 0.0f;                              mat[12] = 0.0f;
	mat[1] = 0.0f;     mat[5] = 1.0f / y; mat[9] = 0.0f;                              mat[13] = 0.0f;
	mat[2] = 0.0f;     mat[6] = 0.0f;     mat[10] = -(zfar + znear) / (zfar - znear); mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
	mat[3] = 0.0f;     mat[7] = 0.0f;     mat[11] = -1.0f;                            mat[15] = 0.0f;
	return mat;
}

inline Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& dir, const Vector3& up)
{
	const Vector3 z = (eye - dir).Normalize();
	const Vector3 x = CrossProduct(up, z).Normalize();
	//const Vector3 y = CrossProduct(z, x).Normalize(); // TODO: удалить?
	const Vector3 y = CrossProduct(z, x);

	Matrix4 m0;
	m0[0] = x.x; m0[4] = x.y; m0[ 8] = x.z; m0[12] = -DotProduct(x, eye);
	m0[1] = y.x; m0[5] = y.y; m0[ 9] = y.z; m0[13] = -DotProduct(y, eye);
	m0[2] = z.x; m0[6] = z.y; m0[10] = z.z; m0[14] = -DotProduct(z, eye);
	m0[3] = 0.0; m0[7] = 0.0; m0[11] = 0.0; m0[15] = 1.0;
	return m0;
}