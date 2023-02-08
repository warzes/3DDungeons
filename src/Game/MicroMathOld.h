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

//то ничего не надо транспонировать - вот у тебя обычная modelview матрица :
//
//float mv[16];
//
//столбец номер 0 - mv[0], mv[1], mv[2], mv[3]
//столбец номер 1 - mv[4], mv[5], mv[6], mv[7]
//и т.д.
//translation часть находится в столбце номер 3 - mv[12], mv[13], mv[14], mv[15]
//
//в шейдере преобразование вершины этой матрицей идет вот так :
//
//mat4 mv;
//vec4 v = mv * gl_Vertex;
//
//при этом mv[0] - столбец 0, mv[1] - столбец 1 и т.д.
//
//
//
//А может я неправильно делал транслейт? надо проверить тестом


// translate должна быть в mv[12], mv[13], mv[14], mv[15]


//=============================================================================
// Header
//=============================================================================
#include <math.h>

//=============================================================================
// Core
//=============================================================================
constexpr const float    DEG2RAD_2 = PI / 360.0f; // DEG2RAD / 2.0f

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

//=============================================================================
// Vector3
//=============================================================================
class Vector3Old
{
public:
	static const Vector3Old Zero;
	static const Vector3Old One;
	static const Vector3Old Left;
	static const Vector3Old Right;
	static const Vector3Old Up;
	static const Vector3Old Down;
	static const Vector3Old Forward;
	static const Vector3Old Back;

	constexpr Vector3Old() = default;
	constexpr Vector3Old(Vector3Old&&) = default;
	constexpr Vector3Old(const Vector3Old&) = default;
	constexpr Vector3Old(float n) : x(n), y(n), z(n) {}
	constexpr Vector3Old(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	constexpr Vector3Old& operator=(Vector3Old&&) = default;
	constexpr Vector3Old& operator=(const Vector3Old&) = default;

	bool operator==(const Vector3Old& v) const { return Equals(x, v.x) && Equals(y, v.y) && Equals(z, v.z); }

	Vector3Old operator-() const noexcept { return { -x, -y, -z }; }
	friend Vector3Old operator-(const Vector3Old& u, float num) noexcept        { return { u.x - num, u.y - num, u.z - num }; }
	friend Vector3Old operator-(float num, const Vector3Old& u) noexcept        { return { num - u.x, num - u.y, num - u.z }; }
	friend Vector3Old operator-(const Vector3Old& u, const Vector3Old& v) noexcept { return { u.x - v.x, u.y - v.y, u.z - v.z }; }
	friend Vector3Old operator+(const Vector3Old& u, float num) noexcept        { return { u.x + num, u.y + num, u.z + num }; }
	friend Vector3Old operator+(float num, const Vector3Old& u) noexcept        { return { num + u.x, num + u.y, num + u.z }; }
	friend Vector3Old operator+(const Vector3Old& u, const Vector3Old& v) noexcept { return { u.x + v.x, u.y + v.y, u.z + v.z }; }
	friend Vector3Old operator*(const Vector3Old& u, float num) noexcept        { return { u.x * num, u.y * num, u.z * num }; }
	friend Vector3Old operator*(float num, const Vector3Old& u) noexcept        { return { num * u.x, num * u.y, num * u.z }; }
	friend Vector3Old operator*(const Vector3Old& u, const Vector3Old& v) noexcept { return { u.x * v.x, u.y * v.y, u.z * v.z }; }
	friend Vector3Old operator/(const Vector3Old& u, float num) noexcept        { return { u.x / num, u.y / num, u.z / num }; }
	friend Vector3Old operator/(float num, const Vector3Old& u) noexcept        { return { num / u.x, num / u.y, num / u.z }; }
	friend Vector3Old operator/(const Vector3Old& u, const Vector3Old& v) noexcept { return { u.x / v.x, u.y / v.y, u.z / v.z }; }

	Vector3Old& operator-=(float num) noexcept        { x -= num; y -= num; z -= num; return *this; }
	Vector3Old& operator-=(const Vector3Old& u) noexcept { x -= u.x; y -= u.y; z -= u.z; return *this; }
	Vector3Old& operator+=(float num) noexcept        { x += num; y += num; z += num; return *this; }
	Vector3Old& operator+=(const Vector3Old& u) noexcept { x += u.x; y += u.y; z += u.z; return *this; }
	Vector3Old& operator*=(float num) noexcept        { x *= num; y *= num; z *= num; return *this; }
	Vector3Old& operator*=(const Vector3Old& u) noexcept { x *= u.x; y *= u.y; z *= u.z; return *this; }
	Vector3Old& operator/=(float num) noexcept        { x /= num; y /= num; z /= num; return *this; }
	Vector3Old& operator/=(const Vector3Old& u) noexcept { x /= u.x; y /= u.y; z /= u.z; return *this; }

	float& operator[](size_t i) noexcept { return (&x)[i]; }
	const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector3Old GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline float Distance(const Vector3Old& v1, const Vector3Old& v2);
inline float DotProduct(const Vector3Old& v1, const Vector3Old& v2);
inline Vector3Old CrossProduct(const Vector3Old& v1, const Vector3Old& v2);
inline Vector3Old Min(const Vector3Old& v1, const Vector3Old& v2);
inline Vector3Old Max(const Vector3Old& v1, const Vector3Old& v2);
inline Vector3Old Lerp(const Vector3Old& a, const Vector3Old& b, float x);
inline Vector3Old Mix(const Vector3Old& u, const Vector3Old& v, float t);
inline Vector3Old Rotate(const Vector3Old& u, float angle, const Vector3Old& v);
// Return the angle between this vector and another vector in degrees.
inline float Angle(const Vector3Old& v1, const Vector3Old& v2);

//=============================================================================
// Vector4
//=============================================================================
class Vector4Old
{
public:
	constexpr Vector4Old() = default;
	constexpr Vector4Old(Vector4Old&&) = default;
	constexpr Vector4Old(const Vector4Old&) = default;
	constexpr Vector4Old(float n) : x(n), y(n), z(n), w(n) {}
	constexpr Vector4Old(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
	constexpr Vector4Old(const Vector3Old& v, float nw) : x(v.x), y(v.y), z(v.z), w(nw) {}

	constexpr Vector4Old& operator=(Vector4Old&&) = default;
	constexpr Vector4Old& operator=(const Vector4Old&) = default;

	bool operator==(const Vector4Old& v) const { return Equals(x, v.x) && Equals(y, v.y) && Equals(z, v.z) && Equals(w, v.w); }

	Vector4Old operator-() const noexcept { return { -x, -y, -z, -w }; }
	friend Vector4Old operator-(const Vector4Old& u, float num) noexcept        { return { u.x - num, u.y - num, u.z - num, u.w - num }; }
	friend Vector4Old operator-(float num, const Vector4Old& u) noexcept        { return { num - u.x, num - u.y, num - u.z, num - u.w }; }
	friend Vector4Old operator-(const Vector4Old& u, const Vector4Old& v) noexcept { return { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w }; }
	friend Vector4Old operator+(const Vector4Old& u, float num) noexcept        { return { u.x + num, u.y + num, u.z + num, u.w + num }; }
	friend Vector4Old operator+(float num, const Vector4Old& u) noexcept        { return { num + u.x, num + u.y, num + u.z, num + u.w }; }
	friend Vector4Old operator+(const Vector4Old& u, const Vector4Old& v) noexcept { return { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w }; }
	friend Vector4Old operator*(const Vector4Old& u, float num) noexcept        { return { u.x * num, u.y * num, u.z * num, u.w * num }; }
	friend Vector4Old operator*(float num, const Vector4Old& u) noexcept        { return { num * u.x, num * u.y, num * u.z, num * u.w }; }
	friend Vector4Old operator*(const Vector4Old& u, const Vector4Old& v) noexcept { return { u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w }; }
	friend Vector4Old operator/(const Vector4Old& u, float num) noexcept        { return { u.x / num, u.y / num, u.z / num, u.w / num }; }
	friend Vector4Old operator/(float num, const Vector4Old& u) noexcept        { return { num / u.x, num / u.y, num / u.z, num / u.w }; }
	friend Vector4Old operator/(const Vector4Old& u, const Vector4Old& v) noexcept { return { u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w }; }

	Vector4Old& operator-=(float num) noexcept        { x -= num; y -= num; z -= num; w -= num; return *this; }
	Vector4Old& operator-=(const Vector4Old& u) noexcept { x -= u.x; y -= u.y; z -= u.z; w -= u.w; return *this; }
	Vector4Old& operator+=(float num) noexcept        { x += num; y += num; z += num; w += num; return *this; }
	Vector4Old& operator+=(const Vector4Old& u) noexcept { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }
	Vector4Old& operator*=(float num) noexcept        { x *= num; y *= num; z *= num; w *= num; return *this; }
	Vector4Old& operator*=(const Vector4Old& u) noexcept { x *= u.x; y *= u.y; z *= u.z; w *= u.w; return *this; }
	Vector4Old& operator/=(float num) noexcept        { x /= num; y /= num; z /= num; w /= num; return *this; }
	Vector4Old& operator/=(const Vector4Old& u) noexcept { x /= u.x; y /= u.y; z /= u.z; w /= u.w; return *this; }

	float& operator[](size_t i) noexcept { return (&x)[i]; }
	const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float* operator&() { return (float*)this; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector4Old GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

inline float DotProduct(const Vector4Old& v1, const Vector4Old& v2);

//=============================================================================
// Quaternion
//=============================================================================
class Matrix3Old;

class QuaternionOld
{
public:
	static const QuaternionOld Identity;

	constexpr QuaternionOld() = default;
	constexpr QuaternionOld(QuaternionOld&&) = default;
	constexpr QuaternionOld(const QuaternionOld&) = default;
	constexpr QuaternionOld(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
	QuaternionOld(float angle, const Vector3Old& axis);         // Construct from an angle (in degrees) and axis.
	QuaternionOld(float ax, float ay, float az);             // Construct from Euler angles (in degrees.)
	QuaternionOld(const Vector3Old& start, const Vector3Old& end); // Construct from the rotation difference between two direction vectors.
	QuaternionOld(const Vector3Old& xAxis, const Vector3Old& yAxis, const Vector3Old& zAxis); // Construct from orthonormal axes.
	QuaternionOld(const Matrix3Old& matrix);                    // Construct from a rotation matrix.

	constexpr QuaternionOld& operator=(QuaternionOld&&) = default;
	constexpr QuaternionOld& operator=(const QuaternionOld&) = default;

	bool operator==(const QuaternionOld& v) const { return Equals(x, v.x) && Equals(y, v.y) && Equals(z, v.z) && Equals(w, v.w); }

	QuaternionOld operator-() const noexcept { return { -x, -y, -z, -w }; }
	friend QuaternionOld operator-(const QuaternionOld& u, const QuaternionOld& v) noexcept { return { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w }; }
	friend QuaternionOld operator+(const QuaternionOld& u, const QuaternionOld& v) noexcept { return { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w }; }
	friend QuaternionOld operator*(const QuaternionOld& u, float num) noexcept { return { u.x * num, u.y * num, u.z * num, u.w * num }; }
	friend QuaternionOld operator*(float num, const QuaternionOld& u) noexcept { return { num * u.x, num * u.y, num * u.z, num * u.w }; }
	friend QuaternionOld operator*(const QuaternionOld& u, const QuaternionOld& v);
	Vector3Old operator*(const Vector3Old& q) const;

	QuaternionOld& operator*=(float num) noexcept { x *= num; y *= num; z *= num; w *= num; return *this; }
	QuaternionOld& operator+=(const QuaternionOld& u) noexcept { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }

	constexpr void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
	
	// Define from an angle (in degrees) and axis.
	void FromAngleAxis(float angle, const Vector3Old& axis);
	// Define from Euler angles (in degrees.)
	void FromEulerAngles(float x, float y, float z);
	// Define from the rotation difference between two direction vectors.
	void FromRotationTo(const Vector3Old& start, const Vector3Old& end);
	// Define from orthonormal axes.
	void FromAxes(const Vector3Old& xAxis, const Vector3Old& yAxis, const Vector3Old& zAxis);
	// Define from a rotation matrix.
	void FromRotationMatrix(const Matrix3Old& matrix);
	// Define from a direction to look in and an up direction. Return true on success, or false if would result in a NaN, in which case the current value remains.
	bool FromLookRotation(const Vector3Old& direction, const Vector3Old& up = Vector3Old::Up);
		
	/// Return whether is NaN.
	bool IsNaN() const { return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z) || ::IsNaN(w); }

	float GetLength() const;
	float GetLengthSquared() const;
	QuaternionOld GetNormalize() const;
	QuaternionOld Inverse() const;
	QuaternionOld Conjugate() const { return QuaternionOld(-x, -y, -z, w); }

	// Return Euler angles in degrees.
	Vector3Old EulerAngles() const;
	// Return yaw angle in degrees.
	float YawAngle() const;
	// Return pitch angle in degrees.
	float PitchAngle() const;
	// Return roll angle in degrees.
	float RollAngle() const;
	// Return the rotation matrix that corresponds to this quaternion.
	Matrix3Old RotationMatrix() const;
	// Spherical interpolation with another quaternion.
	QuaternionOld Slerp(QuaternionOld rhs, float t) const;
	// Normalized linear interpolation with another quaternion.
	QuaternionOld Nlerp(QuaternionOld rhs, float t, bool shortestPath = false) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

inline float DotProduct(const QuaternionOld& v1, const QuaternionOld& v2);

//=============================================================================
// Matrix3
//=============================================================================

class Matrix4Old;

// 3x3 matrix for rotation and scaling.
class Matrix3Old
{
public:
	constexpr Matrix3Old() = default;
	constexpr Matrix3Old(Matrix3Old&&) = default;
	constexpr Matrix3Old(const Matrix3Old&) = default;
	constexpr Matrix3Old(const float* f);
	constexpr Matrix3Old(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);

	constexpr Matrix3Old& operator=(Matrix3Old&&) = default;
	constexpr Matrix3Old& operator=(const Matrix3Old&) = default;

	constexpr float& operator[](size_t i) noexcept { return m[i]; }
	constexpr const float operator[](size_t i) const noexcept { return m[i]; }

	friend Matrix3Old operator+(const Matrix3Old &m1, const Matrix3Old &m2);
	friend Matrix3Old operator-(const Matrix3Old &m1, const Matrix3Old &m2);
	friend Matrix3Old operator*(float f, const Matrix3Old &m);
	friend Matrix3Old operator*(const Matrix3Old &m, float f);
	friend Vector3Old operator*(const Matrix3Old &m, const Vector3Old &u);
	friend Matrix3Old operator*(const Matrix3Old &m1, const Matrix3Old &m2);

	constexpr void Set(const float* f);
	constexpr void Set(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);
	constexpr void Set(const Matrix3Old& M);

	void SetScale(const Vector3Old& scale);
	void SetScale(float scale);

	[[nodiscard]] Vector3Old GetScale() const;
	[[nodiscard]] Matrix3Old Scaled(const Vector3Old& scale) const;

	[[nodiscard]] Matrix3Old Inverse() const;
	[[nodiscard]] Matrix3Old Transpose() const;

	float m[9] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
};

//=============================================================================
// Matrix3x4
//=============================================================================
// 3x4 matrix for scene node transform calculations.
class Matrix3x4Old
{
public:
	constexpr Matrix3x4Old() = default;
	constexpr Matrix3x4Old(Matrix3x4Old&&) = default;
	constexpr Matrix3x4Old(const Matrix3x4Old&) = default;
	constexpr Matrix3x4Old(const float* f);
	constexpr Matrix3x4Old(const Matrix3Old& matrix);
	constexpr Matrix3x4Old(const Matrix4Old& matrix);
	Matrix3x4Old(float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23);
	Matrix3x4Old(const Vector3Old& translation, const QuaternionOld& rotation, float scale);
	Matrix3x4Old(const Vector3Old& translation, const QuaternionOld& rotation, const Vector3Old& scale);

	constexpr Matrix3x4Old& operator=(Matrix3x4Old&&) = default;
	constexpr Matrix3x4Old& operator=(const Matrix3x4Old&) = default;
	Matrix3x4Old& operator=(const Matrix3Old& rhs);
	Matrix3x4Old& operator=(const Matrix4Old& rhs);

	friend Matrix3x4Old operator+(const Matrix3x4Old &m1, const Matrix3x4Old &m2);
	friend Matrix3x4Old operator-(const Matrix3x4Old &m1, const Matrix3x4Old &m2);
	friend Matrix3x4Old operator*(float f, const Matrix3x4Old &m);
	friend Matrix3x4Old operator*(const Matrix3x4Old &m, float f);
	friend Vector3Old operator*(const Matrix3x4Old &m, const Vector3Old &u);
	friend Vector3Old operator*(const Matrix3x4Old &m, const Vector4Old &u);
	friend Matrix3x4Old operator*(const Matrix3x4Old &m1, const Matrix3x4Old &m2);
	friend Matrix4Old operator*(const Matrix3x4Old &m1, const Matrix4Old &m2);
	friend Matrix4Old operator*(const Matrix4Old &m1, const Matrix3x4Old &m2);

	constexpr float& operator[](size_t i) { return m[i]; }
	constexpr const float operator[](size_t i) const { return m[i]; }

	void SetTranslation(const Vector3Old& translation);
	void SetRotation(const Matrix3Old& rotation);
	void SetScale(const Vector3Old& scale);
	void SetScale(float scale);

	[[nodiscard]] Matrix3Old ToMatrix3() const;
	[[nodiscard]] Matrix4Old ToMatrix4() const;

	[[nodiscard]] Matrix3Old RotationMatrix() const;
	[[nodiscard]] Vector3Old GetTranslation() const;
	[[nodiscard]] QuaternionOld GetRotation() const { return QuaternionOld(RotationMatrix()); }
	[[nodiscard]] Vector3Old GetScale() const;

	void Decompose(Vector3Old& translation, QuaternionOld& rotation, Vector3Old& scale) const;
	Matrix3x4Old Inverse() const;

	float m[12] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f
	};
};

//=============================================================================
// Matrix4
//=============================================================================

class Matrix4Old
{
public:
	constexpr Matrix4Old() = default;
	constexpr Matrix4Old(Matrix4Old&&) = default;
	constexpr Matrix4Old(const Matrix4Old&) = default;
	constexpr Matrix4Old(const float* f);
	constexpr Matrix4Old(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15);
	constexpr Matrix4Old(const Matrix3Old &mat);
	constexpr Matrix4Old(const Matrix3x4Old &mat);

	constexpr Matrix4Old& operator=(Matrix4Old&&) = default;
	constexpr Matrix4Old& operator=(const Matrix4Old&) = default;
	Matrix4Old& operator=(const Matrix3Old& rhs);

	constexpr float& operator[](size_t i) noexcept { return m[i]; }
	constexpr const float operator[](size_t i) const noexcept { return m[i]; }

	friend Matrix4Old operator+(const Matrix4Old &m1, const Matrix4Old &m2);
	friend Matrix4Old operator-(const Matrix4Old &m1, const Matrix4Old &m2);
	friend Matrix4Old operator*(const Matrix4Old &m, float f);
	friend Matrix4Old operator*(float f, const Matrix4Old &m);
	friend Vector3Old operator*(const Matrix4Old &m, const Vector3Old &u);
	friend Vector4Old operator*(const Matrix4Old &m, const Vector4Old &u);
	friend Matrix4Old operator*(const Matrix4Old &m1, const Matrix4Old &m2);

	constexpr void Set(const float* f);
	constexpr void Set(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15);
	constexpr void Set(const Matrix4Old& M);

	void SetTranslation(const Vector3Old& translation);
	void SetRotation(const Matrix3Old& rotation);
	void SetScale(const Vector3Old& scale);
	void SetScale(float scale);

	[[nodiscard]] Matrix3Old ToMatrix3() const;

	// Return the rotation matrix with scaling removed.
	[[nodiscard]] Matrix3Old RotationMatrix() const;

	[[nodiscard]] Vector3Old GetTranslation() const;
	[[nodiscard]] QuaternionOld GetRotation() const;
	[[nodiscard]] Vector3Old GetScale() const;

	[[nodiscard]] Matrix4Old Inverse() const;
	[[nodiscard]] Matrix4Old Transpose() const;

	void Decompose(Vector3Old& translation, QuaternionOld& rotation, Vector3Old& scale) const;


	// эти функции расчитаны на строки, а я переделал под столбцы
	// или все таки эти функции оставить, тогда надо переделать умножения
	[[nodiscard]] static Matrix4Old Rotate(const Vector3Old& axis, float angle);
	[[nodiscard]] static Matrix4Old RotateX(float angle);
	[[nodiscard]] static Matrix4Old RotateY(float angle);
	[[nodiscard]] static Matrix4Old RotateZ(float angle);
	[[nodiscard]] static Matrix4Old Scale(const Vector3Old& v);
	[[nodiscard]] static Matrix4Old Translate(const Vector3Old& v);

	[[nodiscard]] static Matrix4Old Ortho(float left, float right, float bottom, float top, float n, float f);
	[[nodiscard]] static Matrix4Old Perspective(float fieldOfView, float aspectRatio, float znear, float zfar);
	[[nodiscard]] static Matrix4Old LookAt(const Vector3Old& eye, const Vector3Old& dir, const Vector3Old& up);

	float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
};

#include "MicroMathOld.inl"