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
constexpr const float EPSILON         = 1.0e-6f;
constexpr const float PI              = 3.14159265358979323846f;
constexpr const float HALF_PI         = PI * 0.5f;
constexpr const float DEG2RAD         = PI / 180.0f;
constexpr const float DEG2RAD_2       = PI / 360.0f; // DEG2RAD / 2.f
constexpr const float RAD2DEG         = 180.0f / PI;
constexpr const int MIN_INT           = 0x80000000;
constexpr const int MAX_INT           = 0x7fffffff;
constexpr const unsigned MIN_UNSIGNED = 0x00000000;
constexpr const unsigned MAX_UNSIGNED = 0xffffffff;
constexpr const float MAX_FLOAT       = 3.402823466e+38f;

// Check whether two floating point values are equal within accuracy.
inline bool Equals(float x, float y, float epsilon = EPSILON) noexcept { return fabsf(x - y) < epsilon; }
// Check whether a floating point value is NaN.
inline bool IsNaN(float value) noexcept { return value != value; }

inline constexpr float ConvertToRadians(float degrees) noexcept { return degrees * DEG2RAD; }
inline constexpr float ConvertToDegrees(float radians) noexcept { return radians * RAD2DEG; }

inline constexpr int   Abs(int value) noexcept { return value >= 0 ? value : -value; }
inline constexpr float Abs(float value) noexcept { return value >= 0.0f ? value : -value; }
inline constexpr int   Min(int a, int b) noexcept { return a < b ? a : b; }
inline constexpr float Min(float a, float b) noexcept { return a < b ? a : b; }
inline constexpr int   Max(int a, int b) noexcept { return a > b ? a : b; }
inline constexpr float Max(float a, float b) noexcept { return a > b ? a : b; }

inline constexpr float Clamp(float x, float min, float max) noexcept
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Linear interpolation between two float values.
inline constexpr float Lerp(float lhs, float rhs, float t) noexcept
{
	return lhs * (1.0f - t) + rhs * t;
}
// ==== Может не нужно?
// Return sine of an angle in degrees.
inline float Sin(float angle) noexcept { return sinf(angle * DEG2RAD); }
// Return cosine of an angle in degrees.
inline float Cos(float angle) noexcept { return cosf(angle * DEG2RAD); }
// Return tangent of an angle in degrees.
inline float Tan(float angle) noexcept { return tanf(angle * DEG2RAD); }
// Return arc sine in degrees.
inline float Asin(float x) noexcept { return RAD2DEG * asinf(Clamp(x, -1.0f, 1.0f)); }
// Return arc cosine in degrees.
inline float Acos(float x) noexcept { return RAD2DEG * acosf(Clamp(x, -1.0f, 1.0f)); }
// Return arc tangent in degrees.
inline float Atan(float x) noexcept { return RAD2DEG * atanf(x); }
// Return arc tangent of y/x in degrees.
inline float Atan2(float y, float x) noexcept { return RAD2DEG * atan2f(y, x); }
// ==== Может не нужно?
// 
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

	friend Point2 operator-(const Point2& lhs, const Point2& rhs) noexcept { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
	friend Point2 operator+(const Point2& lhs, const Point2& rhs) noexcept { return { lhs.x + rhs.x, lhs.y + rhs.y }; }

	Point2& operator-=(const Point2& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
	Point2& operator+=(const Point2& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }

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

	bool operator==(const Vector2& v) const { return Equals(x, v.x) && Equals(y, v.y); }

	Vector2 operator-() const { return { -x, -y }; }
	friend Vector2 operator-(const Vector2& u, float num) noexcept        { return { u.x - num, u.y - num }; }
	friend Vector2 operator-(float num, const Vector2& u) noexcept        { return { num - u.x, num - u.y }; }
	friend Vector2 operator-(const Vector2& u, const Vector2& v) noexcept { return { u.x - v.x, u.y - v.y }; }
	friend Vector2 operator+(const Vector2& u, float num) noexcept        { return { u.x + num, u.y + num }; }
	friend Vector2 operator+(float num, const Vector2& u) noexcept        { return { num + u.x, num + u.y }; }
	friend Vector2 operator+(const Vector2& u, const Vector2& v) noexcept { return { u.x + v.x, u.y + v.y }; }
	friend Vector2 operator*(const Vector2& u, float num) noexcept        { return { u.x * num, u.y * num }; }
	friend Vector2 operator*(float num, const Vector2& u) noexcept        { return { num * u.x, num * u.y }; }
	friend Vector2 operator*(const Vector2& u, const Vector2& v) noexcept { return { u.x * v.x, u.y * v.y }; }
	friend Vector2 operator/(const Vector2& u, float num) noexcept        { return { u.x / num, u.y / num }; }
	friend Vector2 operator/(float num, const Vector2& u) noexcept        { return { num / u.x, num / u.y }; }
	friend Vector2 operator/(const Vector2& u, const Vector2& v) noexcept { return { u.x / v.x, u.y / v.y }; }

	Vector2& operator-=(float num) noexcept        { x -= num; y -= num; return *this; }
	Vector2& operator-=(const Vector2& u) noexcept { x -= u.x; y -= u.y; return *this; }
	Vector2& operator+=(float num) noexcept        { x += num; y += num; return *this; }
	Vector2& operator+=(const Vector2& u) noexcept { x += u.x; y += u.y; return *this; }
	Vector2& operator*=(float num) noexcept        { x *= num; y *= num; return *this; }
	Vector2& operator*=(const Vector2& u) noexcept { x *= u.x; y *= u.y; return *this; }
	Vector2& operator/=(float num) noexcept        { x /= num; y /= num; return *this; }
	Vector2& operator/=(const Vector2& u) noexcept { x /= u.x; y /= u.y; return *this; }

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

//=============================================================================
// Vector3
//=============================================================================
class Vector3
{
public:
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Left;
	static const Vector3 Right;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Forward;
	static const Vector3 Back;

	constexpr Vector3() = default;
	constexpr Vector3(Vector3&&) = default;
	constexpr Vector3(const Vector3&) = default;
	constexpr Vector3(float n) : x(n), y(n), z(n) {}
	constexpr Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	constexpr Vector3& operator=(Vector3&&) = default;
	constexpr Vector3& operator=(const Vector3&) = default;

	bool operator==(const Vector3& v) const { return Equals(x, v.x) && Equals(y, v.y) && Equals(z, v.z); }

	Vector3 operator-() const noexcept { return { -x, -y, -z }; }
	friend Vector3 operator-(const Vector3& u, float num) noexcept        { return { u.x - num, u.y - num, u.z - num }; }
	friend Vector3 operator-(float num, const Vector3& u) noexcept        { return { num - u.x, num - u.y, num - u.z }; }
	friend Vector3 operator-(const Vector3& u, const Vector3& v) noexcept { return { u.x - v.x, u.y - v.y, u.z - v.z }; }
	friend Vector3 operator+(const Vector3& u, float num) noexcept        { return { u.x + num, u.y + num, u.z + num }; }
	friend Vector3 operator+(float num, const Vector3& u) noexcept        { return { num + u.x, num + u.y, num + u.z }; }
	friend Vector3 operator+(const Vector3& u, const Vector3& v) noexcept { return { u.x + v.x, u.y + v.y, u.z + v.z }; }
	friend Vector3 operator*(const Vector3& u, float num) noexcept        { return { u.x * num, u.y * num, u.z * num }; }
	friend Vector3 operator*(float num, const Vector3& u) noexcept        { return { num * u.x, num * u.y, num * u.z }; }
	friend Vector3 operator*(const Vector3& u, const Vector3& v) noexcept { return { u.x * v.x, u.y * v.y, u.z * v.z }; }
	friend Vector3 operator/(const Vector3& u, float num) noexcept        { return { u.x / num, u.y / num, u.z / num }; }
	friend Vector3 operator/(float num, const Vector3& u) noexcept        { return { num / u.x, num / u.y, num / u.z }; }
	friend Vector3 operator/(const Vector3& u, const Vector3& v) noexcept { return { u.x / v.x, u.y / v.y, u.z / v.z }; }

	Vector3& operator-=(float num) noexcept        { x -= num; y -= num; z -= num; return *this; }
	Vector3& operator-=(const Vector3& u) noexcept { x -= u.x; y -= u.y; z -= u.z; return *this; }
	Vector3& operator+=(float num) noexcept        { x += num; y += num; z += num; return *this; }
	Vector3& operator+=(const Vector3& u) noexcept { x += u.x; y += u.y; z += u.z; return *this; }
	Vector3& operator*=(float num) noexcept        { x *= num; y *= num; z *= num; return *this; }
	Vector3& operator*=(const Vector3& u) noexcept { x *= u.x; y *= u.y; z *= u.z; return *this; }
	Vector3& operator/=(float num) noexcept        { x /= num; y /= num; z /= num; return *this; }
	Vector3& operator/=(const Vector3& u) noexcept { x /= u.x; y /= u.y; z /= u.z; return *this; }

	float& operator[](size_t i) noexcept { return (&x)[i]; }
	const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector3 GetNormalize() const;

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
// Return the angle between this vector and another vector in degrees.
inline float Angle(const Vector3& v1, const Vector3& v2);

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
	constexpr Vector4(const Vector3& v, float nw) : x(v.x), y(v.y), z(v.z), w(nw) {}

	constexpr Vector4& operator=(Vector4&&) = default;
	constexpr Vector4& operator=(const Vector4&) = default;

	bool operator==(const Vector4& v) const { return Equals(x, v.x) && Equals(y, v.y) && Equals(z, v.z) && Equals(w, v.w); }

	Vector4 operator-() const noexcept { return { -x, -y, -z, -w }; }
	friend Vector4 operator-(const Vector4& u, float num) noexcept        { return { u.x - num, u.y - num, u.z - num, u.w - num }; }
	friend Vector4 operator-(float num, const Vector4& u) noexcept        { return { num - u.x, num - u.y, num - u.z, num - u.w }; }
	friend Vector4 operator-(const Vector4& u, const Vector4& v) noexcept { return { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w }; }
	friend Vector4 operator+(const Vector4& u, float num) noexcept        { return { u.x + num, u.y + num, u.z + num, u.w + num }; }
	friend Vector4 operator+(float num, const Vector4& u) noexcept        { return { num + u.x, num + u.y, num + u.z, num + u.w }; }
	friend Vector4 operator+(const Vector4& u, const Vector4& v) noexcept { return { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w }; }
	friend Vector4 operator*(const Vector4& u, float num) noexcept        { return { u.x * num, u.y * num, u.z * num, u.w * num }; }
	friend Vector4 operator*(float num, const Vector4& u) noexcept        { return { num * u.x, num * u.y, num * u.z, num * u.w }; }
	friend Vector4 operator*(const Vector4& u, const Vector4& v) noexcept { return { u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w }; }
	friend Vector4 operator/(const Vector4& u, float num) noexcept        { return { u.x / num, u.y / num, u.z / num, u.w / num }; }
	friend Vector4 operator/(float num, const Vector4& u) noexcept        { return { num / u.x, num / u.y, num / u.z, num / u.w }; }
	friend Vector4 operator/(const Vector4& u, const Vector4& v) noexcept { return { u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w }; }

	Vector4& operator-=(float num) noexcept        { x -= num; y -= num; z -= num; w -= num; return *this; }
	Vector4& operator-=(const Vector4& u) noexcept { x -= u.x; y -= u.y; z -= u.z; w -= u.w; return *this; }
	Vector4& operator+=(float num) noexcept        { x += num; y += num; z += num; w += num; return *this; }
	Vector4& operator+=(const Vector4& u) noexcept { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }
	Vector4& operator*=(float num) noexcept        { x *= num; y *= num; z *= num; w *= num; return *this; }
	Vector4& operator*=(const Vector4& u) noexcept { x *= u.x; y *= u.y; z *= u.z; w *= u.w; return *this; }
	Vector4& operator/=(float num) noexcept        { x /= num; y /= num; z /= num; w /= num; return *this; }
	Vector4& operator/=(const Vector4& u) noexcept { x /= u.x; y /= u.y; z /= u.z; w /= u.w; return *this; }

	float& operator[](size_t i) noexcept { return (&x)[i]; }
	const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float* operator&() { return (float*)this; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector4 GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

inline float DotProduct(const Vector4& v1, const Vector4& v2);

//=============================================================================
// Quaternion
//=============================================================================
class Matrix3;

class Quaternion
{
public:
	static const Quaternion Identity;

	constexpr Quaternion() = default;
	constexpr Quaternion(Quaternion&&) = default;
	constexpr Quaternion(const Quaternion&) = default;
	constexpr Quaternion(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
	Quaternion(float angle, const Vector3& axis);         // Construct from an angle (in degrees) and axis.
	Quaternion(float ax, float ay, float az);             // Construct from Euler angles (in degrees.)
	Quaternion(const Vector3& start, const Vector3& end); // Construct from the rotation difference between two direction vectors.
	Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis); // Construct from orthonormal axes.
	Quaternion(const Matrix3& matrix);                    // Construct from a rotation matrix.

	constexpr Quaternion& operator=(Quaternion&&) = default;
	constexpr Quaternion& operator=(const Quaternion&) = default;

	bool operator==(const Quaternion& v) const { return Equals(x, v.x) && Equals(y, v.y) && Equals(z, v.z) && Equals(w, v.w); }

	Quaternion operator-() const noexcept { return { -x, -y, -z, -w }; }
	friend Quaternion operator-(const Quaternion& u, const Quaternion& v) noexcept { return { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w }; }
	friend Quaternion operator+(const Quaternion& u, const Quaternion& v) noexcept { return { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w }; }
	friend Quaternion operator*(const Quaternion& u, float num) noexcept { return { u.x * num, u.y * num, u.z * num, u.w * num }; }
	friend Quaternion operator*(float num, const Quaternion& u) noexcept { return { num * u.x, num * u.y, num * u.z, num * u.w }; }
	friend Quaternion operator*(const Quaternion& u, const Quaternion& v);
	Vector3 operator*(const Vector3& q) const;

	Quaternion& operator*=(float num) noexcept { x *= num; y *= num; z *= num; w *= num; return *this; }
	Quaternion& operator+=(const Quaternion& u) noexcept { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }

	constexpr void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
	
	// Define from an angle (in degrees) and axis.
	void FromAngleAxis(float angle, const Vector3& axis);
	// Define from Euler angles (in degrees.)
	void FromEulerAngles(float x, float y, float z);
	// Define from the rotation difference between two direction vectors.
	void FromRotationTo(const Vector3& start, const Vector3& end);
	// Define from orthonormal axes.
	void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	// Define from a rotation matrix.
	void FromRotationMatrix(const Matrix3& matrix);
	// Define from a direction to look in and an up direction. Return true on success, or false if would result in a NaN, in which case the current value remains.
	bool FromLookRotation(const Vector3& direction, const Vector3& up = Vector3::Up);
		
	/// Return whether is NaN.
	bool IsNaN() const { return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z) || ::IsNaN(w); }

	float GetLength() const;
	float GetLengthSquared() const;
	Quaternion GetNormalize() const;
	Quaternion Inverse() const;
	Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }

	// Return Euler angles in degrees.
	Vector3 EulerAngles() const;
	// Return yaw angle in degrees.
	float YawAngle() const;
	// Return pitch angle in degrees.
	float PitchAngle() const;
	// Return roll angle in degrees.
	float RollAngle() const;
	// Return the rotation matrix that corresponds to this quaternion.
	Matrix3 RotationMatrix() const;
	// Spherical interpolation with another quaternion.
	Quaternion Slerp(Quaternion rhs, float t) const;
	// Normalized linear interpolation with another quaternion.
	Quaternion Nlerp(Quaternion rhs, float t, bool shortestPath = false) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

inline float DotProduct(const Quaternion& v1, const Quaternion& v2);

//=============================================================================
// Matrix3
//=============================================================================

class Matrix4;

// 3x3 matrix for rotation and scaling.
class Matrix3
{
public:
	constexpr Matrix3() = default;
	constexpr Matrix3(Matrix3&&) = default;
	constexpr Matrix3(const Matrix3&) = default;
	constexpr Matrix3(const float* f);
	constexpr Matrix3(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);

	constexpr Matrix3& operator=(Matrix3&&) = default;
	constexpr Matrix3& operator=(const Matrix3&) = default;

	constexpr float& operator[](size_t i) noexcept { return m[i]; }
	constexpr const float operator[](size_t i) const noexcept { return m[i]; }

	friend Matrix3 operator+(const Matrix3 &m1, const Matrix3 &m2);
	friend Matrix3 operator-(const Matrix3 &m1, const Matrix3 &m2);
	friend Matrix3 operator*(float f, const Matrix3 &m);
	friend Matrix3 operator*(const Matrix3 &m, float f);
	friend Vector3 operator*(const Matrix3 &m, const Vector3 &u);
	friend Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2);

	constexpr void Set(const float* f);
	constexpr void Set(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);
	constexpr void Set(const Matrix3& M);

	void SetScale(const Vector3& scale);
	void SetScale(float scale);

	// Return the scaling part.
	Vector3 GetScale() const;
	Matrix3 Scaled(const Vector3& scale) const;

	[[nodiscard]] Matrix3 Inverse() const;
	[[nodiscard]] Matrix3 Transpose() const;

	float m[9] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
};

//=============================================================================
// Matrix3x4
//=============================================================================
class Matrix3x4
{
public:
	constexpr Matrix3x4() = default;
	constexpr Matrix3x4(Matrix3x4&&) = default;
	constexpr Matrix3x4(const Matrix3x4&) = default;
	constexpr Matrix3x4(const float* f);
	Matrix3x4(const Matrix3& matrix);
	Matrix3x4(const Matrix4& matrix);
	Matrix3x4(float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23);
	Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale);
	Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);

	constexpr Matrix3x4& operator=(Matrix3x4&&) = default;
	constexpr Matrix3x4& operator=(const Matrix3x4&) = default;
	Matrix3x4& operator=(const Matrix3& rhs);
	Matrix3x4& operator=(const Matrix4& rhs);

	friend Matrix3x4 operator+(const Matrix3x4 &m1, const Matrix3x4 &m2);
	friend Matrix3x4 operator-(const Matrix3x4 &m1, const Matrix3x4 &m2);
	friend Matrix3x4 operator*(float f, const Matrix3x4 &m);
	friend Matrix3x4 operator*(const Matrix3x4 &m, float f);
	friend Vector3 operator*(const Matrix3x4 &m, const Vector3 &u);
	friend Vector3 operator*(const Matrix3x4 &m, const Vector4 &u);
	friend Matrix3x4 operator*(const Matrix3x4 &m1, const Matrix3x4 &m2);
	friend Matrix4 operator*(const Matrix3x4 &m1, const Matrix4 &m2);
	friend Matrix4 operator*(const Matrix4 &m1, const Matrix3x4 &m2);

	constexpr float& operator[](size_t i) { return m[i]; }
	constexpr const float operator[](size_t i) const { return m[i]; }

	void SetTranslation(const Vector3& translation);
	void SetRotation(const Matrix3& rotation);
	void SetScale(const Vector3& scale);
	void SetScale(float scale);

	Matrix3 ToMatrix3() const;
	Matrix4 ToMatrix4() const;

	Matrix3 RotationMatrix() const;
	Vector3 Translation() const;
	Quaternion Rotation() const { return Quaternion(RotationMatrix()); }
	Vector3 Scale() const;

	void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;
	Matrix3x4 Inverse() const;

	float m[12] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f
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

	void Decompose(Vector3& outScale, Quaternion& outRotation, Vector3& outTranslation) const;

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

#include "MicroMath.inl"