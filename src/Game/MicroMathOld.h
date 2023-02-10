#pragma once

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
	QuaternionOld(float angle, const Vector3& axis);         // Construct from an angle (in degrees) and axis.
	QuaternionOld(float ax, float ay, float az);             // Construct from Euler angles (in degrees.)
	QuaternionOld(const Vector3& start, const Vector3& end); // Construct from the rotation difference between two direction vectors.
	QuaternionOld(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis); // Construct from orthonormal axes.
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
	Vector3 operator*(const Vector3& q) const;

	QuaternionOld& operator*=(float num) noexcept { x *= num; y *= num; z *= num; w *= num; return *this; }
	QuaternionOld& operator+=(const QuaternionOld& u) noexcept { x += u.x; y += u.y; z += u.z; w += u.w; return *this; }

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
	void FromRotationMatrix(const Matrix3Old& matrix);
	// Define from a direction to look in and an up direction. Return true on success, or false if would result in a NaN, in which case the current value remains.
	bool FromLookRotation(const Vector3& direction, const Vector3& up = Vector3::Up);
		
	/// Return whether is NaN.
	bool IsNaN() const { return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z) || ::IsNaN(w); }

	float GetLength() const;
	float GetLengthSquared() const;
	QuaternionOld GetNormalize() const;
	QuaternionOld Inverse() const;
	QuaternionOld Conjugate() const { return QuaternionOld(-x, -y, -z, w); }

	// Return Euler angles in degrees.
	Vector3 EulerAngles() const;
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
	friend Vector3 operator*(const Matrix3Old &m, const Vector3 &u);
	friend Matrix3Old operator*(const Matrix3Old &m1, const Matrix3Old &m2);

	constexpr void Set(const float* f);
	constexpr void Set(
		float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);
	constexpr void Set(const Matrix3Old& M);

	void SetScale(const Vector3& scale);
	void SetScale(float scale);

	[[nodiscard]] Vector3 GetScale() const;
	[[nodiscard]] Matrix3Old Scaled(const Vector3& scale) const;

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
	Matrix3x4Old(const Vector3& translation, const QuaternionOld& rotation, float scale);
	Matrix3x4Old(const Vector3& translation, const QuaternionOld& rotation, const Vector3& scale);

	constexpr Matrix3x4Old& operator=(Matrix3x4Old&&) = default;
	constexpr Matrix3x4Old& operator=(const Matrix3x4Old&) = default;
	Matrix3x4Old& operator=(const Matrix3Old& rhs);
	Matrix3x4Old& operator=(const Matrix4Old& rhs);

	friend Matrix3x4Old operator+(const Matrix3x4Old &m1, const Matrix3x4Old &m2);
	friend Matrix3x4Old operator-(const Matrix3x4Old &m1, const Matrix3x4Old &m2);
	friend Matrix3x4Old operator*(float f, const Matrix3x4Old &m);
	friend Matrix3x4Old operator*(const Matrix3x4Old &m, float f);
	friend Vector3 operator*(const Matrix3x4Old &m, const Vector3 &u);
	friend Vector3 operator*(const Matrix3x4Old &m, const Vector4 &u);
	friend Matrix3x4Old operator*(const Matrix3x4Old &m1, const Matrix3x4Old &m2);
	friend Matrix4Old operator*(const Matrix3x4Old &m1, const Matrix4Old &m2);
	friend Matrix4Old operator*(const Matrix4Old &m1, const Matrix3x4Old &m2);

	constexpr float& operator[](size_t i) { return m[i]; }
	constexpr const float operator[](size_t i) const { return m[i]; }

	void SetTranslation(const Vector3& translation);
	void SetRotation(const Matrix3Old& rotation);
	void SetScale(const Vector3& scale);
	void SetScale(float scale);

	[[nodiscard]] Matrix3Old ToMatrix3() const;
	[[nodiscard]] Matrix4Old ToMatrix4() const;

	[[nodiscard]] Matrix3Old RotationMatrix() const;
	[[nodiscard]] Vector3 GetTranslation() const;
	[[nodiscard]] QuaternionOld GetRotation() const { return QuaternionOld(RotationMatrix()); }
	[[nodiscard]] Vector3 GetScale() const;

	void Decompose(Vector3& translation, QuaternionOld& rotation, Vector3& scale) const;
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
	friend Vector3 operator*(const Matrix4Old &m, const Vector3 &u);
	friend Vector4 operator*(const Matrix4Old &m, const Vector4 &u);
	friend Matrix4Old operator*(const Matrix4Old &m1, const Matrix4Old &m2);

	constexpr void Set(const float* f);
	constexpr void Set(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15);
	constexpr void Set(const Matrix4Old& M);

	void SetTranslation(const Vector3& translation);
	void SetRotation(const Matrix3Old& rotation);
	void SetScale(const Vector3& scale);
	void SetScale(float scale);

	[[nodiscard]] Matrix3Old ToMatrix3() const;

	// Return the rotation matrix with scaling removed.
	[[nodiscard]] Matrix3Old RotationMatrix() const;

	[[nodiscard]] Vector3 GetTranslation() const;
	[[nodiscard]] QuaternionOld GetRotation() const;
	[[nodiscard]] Vector3 GetScale() const;

	[[nodiscard]] Matrix4Old Inverse() const;
	[[nodiscard]] Matrix4Old Transpose() const;

	void Decompose(Vector3& translation, QuaternionOld& rotation, Vector3& scale) const;


	// эти функции расчитаны на строки, а я переделал под столбцы
	// или все таки эти функции оставить, тогда надо переделать умножения
	[[nodiscard]] static Matrix4Old Rotate(const Vector3& axis, float angle);
	[[nodiscard]] static Matrix4Old RotateX(float angle);
	[[nodiscard]] static Matrix4Old RotateY(float angle);
	[[nodiscard]] static Matrix4Old RotateZ(float angle);
	[[nodiscard]] static Matrix4Old Scale(const Vector3& v);
	[[nodiscard]] static Matrix4Old Translate(const Vector3& v);

	[[nodiscard]] static Matrix4Old Ortho(float left, float right, float bottom, float top, float n, float f);
	[[nodiscard]] static Matrix4Old Perspective(float fieldOfView, float aspectRatio, float znear, float zfar);
	[[nodiscard]] static Matrix4Old LookAt(const Vector3& eye, const Vector3& dir, const Vector3& up);

	float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
};

#include "MicroMathOld.inl"