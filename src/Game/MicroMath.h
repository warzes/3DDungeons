#pragma once

/*
* Matrix Column Row
* 
Left handed
  1 Y   Z
    |  / 1
    | /
    |/___X
   0     1
*/

//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4514)
#	pragma warning(disable : 5264)
#endif // _MSC_VER
#include <math.h>
#include <limits>
#include <assert.h>

//=============================================================================
// Constant definitions
//=============================================================================
constexpr const float    EPSILON = 1.0e-6f; // TODO: заменить на std::numeric_limits<float>::epsilon()
constexpr const float    PI = 3.14159265358979323846f;
constexpr const float    HALF_PI = PI * 0.5f;
constexpr const float    DEG2RAD = PI / 180.0f;
constexpr const float    RAD2DEG = 180.0f / PI;
constexpr const float    CosOneOverTwo = float(0.877582561890372716130286068203503191);

class Color;
class Point2;
class Vector2;
class Vector3;
class Vector4;
class Quaternion;
class Matrix3;
class Matrix4;

//=============================================================================
// Core functions
//=============================================================================

inline bool Equals(float x, float y, float epsilon = EPSILON) noexcept; // Check whether two floating point values are equal within accuracy.
inline constexpr bool  IsNaN(float value) noexcept; // Check whether a floating point value is NaN.
inline constexpr float ConvertToRadians(float degrees) noexcept;
inline constexpr float ConvertToDegrees(float radians) noexcept;
inline constexpr int   Min(int a, int b) noexcept;
inline constexpr float Min(float a, float b) noexcept;
inline constexpr int   Max(int a, int b) noexcept;
inline constexpr float Max(float a, float b) noexcept;
inline constexpr float Clamp(float value, float min, float max) noexcept;
inline constexpr float Lerp(float x, float y, float f) noexcept; // Linear interpolation between two float values.
inline constexpr float Mix(float x, float y, float a) noexcept;

//=============================================================================
// Color
//=============================================================================
class Color
{
public:
	constexpr Color() = default;
	constexpr Color(Color&&) = default;
	constexpr Color(const Color&) = default;
	constexpr Color(unsigned uintRGBA);
	constexpr Color(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na = 255) : r(nr), g(ng), b(nb), a(na) {}

	constexpr Color& operator=(Color&&) = default;
	constexpr Color& operator=(const Color&) = default;

	unsigned ToUInt() const { return (unsigned)a << 24 | r << 16 | g << 8 | b; }

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
};

inline bool operator==(const Color& Left, const Color& Right) noexcept;

// Пока не реализованно, при этом функции должны клампится
//inline Color operator-(const Color& Left, const Color& Right) noexcept;
//inline Color operator+(const Color& Left, const Color& Right) noexcept;
//inline Color operator*(const Color& Left, const Color& Right) noexcept;
//inline Color& operator-=(Color& Left, const Color& Right) noexcept;
//inline Color& operator+=(Color& Left, const Color& Right) noexcept;
//inline Color& operator*=(Color& Left, const Color& Right) noexcept;

//=============================================================================
// Point2
//=============================================================================
class Point2
{
public:
	constexpr Point2() = default;
	constexpr Point2(Point2&&) = default;
	constexpr Point2(const Point2&) = default;
	constexpr Point2(int scalar) : x(scalar), y(scalar) {}
	constexpr Point2(int nx, int ny) : x(nx), y(ny) {}

	constexpr Point2& operator=(Point2&&) = default;
	constexpr Point2& operator=(const Point2&) = default;

	int x = 0;
	int y = 0;
};

inline bool operator==(const Point2& Left, const Point2& Right) noexcept;

inline Point2 operator-(const Point2& In) noexcept;
inline Point2 operator-(int Left, const Point2& Right) noexcept;
inline Point2 operator-(const Point2& Left, int Right) noexcept;
inline Point2 operator-(const Point2& Left, const Point2& Right) noexcept;
inline Point2 operator+(int Left, const Point2& Right) noexcept;
inline Point2 operator+(const Point2& Left, int Right) noexcept;
inline Point2 operator+(const Point2& Left, const Point2& Right) noexcept;
inline Point2 operator*(int Left, const Point2& Right) noexcept;
inline Point2 operator*(const Point2& Left, int Right) noexcept;
inline Point2 operator*(const Point2& Left, const Point2& Right) noexcept;
inline Point2 operator/(int Left, const Point2& Right) noexcept;
inline Point2 operator/(const Point2& Left, int Right) noexcept;
inline Point2 operator/(const Point2& Left, const Point2& Right) noexcept;

inline Point2& operator-=(Point2& Left, int Right) noexcept;
inline Point2& operator-=(Point2& Left, const Point2& Right) noexcept;
inline Point2& operator+=(Point2& Left, int Right) noexcept;
inline Point2& operator+=(Point2& Left, const Point2& Right) noexcept;
inline Point2& operator*=(Point2& Left, int Right) noexcept;
inline Point2& operator*=(Point2& Left, const Point2& Right) noexcept;
inline Point2& operator/=(Point2& Left, int Right) noexcept;
inline Point2& operator/=(Point2& Left, const Point2& Right) noexcept;

//=============================================================================
// Vector2
//=============================================================================
class Vector2
{
public:
	constexpr Vector2() = default;
	constexpr Vector2(Vector2&&) = default;
	constexpr Vector2(const Vector2&) = default;
	constexpr Vector2(float scalar) : x(scalar), y(scalar) {}
	constexpr Vector2(float nx, float ny) : x(nx), y(ny) {}

	constexpr Vector2& operator=(Vector2&&) = default;
	constexpr Vector2& operator=(const Vector2&) = default;

	constexpr float& operator[](size_t i) noexcept { return (&x)[i]; }
	constexpr const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector2 GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
};

inline bool Equals(const Vector2& v1, const Vector2& v2, float epsilon = EPSILON) noexcept;

inline float Distance(const Vector2& v1, const Vector2& v2);
inline float DotProduct(const Vector2& v1, const Vector2& v2);
inline Vector2 Reflect(const Vector2& v, const Vector2& normal);
inline Vector2 Refract(const Vector2& i, const Vector2& normal, float eta);
inline Vector2 Min(const Vector2& v1, const Vector2& v2);
inline Vector2 Max(const Vector2& v1, const Vector2& v2);
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x);
inline Vector2 Mix(const Vector2& x, const Vector2& y, float a);

// Rotate a two dimensional vector.
inline Vector2 Rotate(const Vector2& v, float angle);

// Returns the absolute angle between two vectors.
inline float Angle(const Vector2& x, const Vector2& y);

// Returns the oriented angle between two 2d vectors.
inline float OrientedAngle(const Vector2& x, const Vector2& y);

inline bool operator==(const Vector2& Left, const Vector2& Right) noexcept;
inline bool operator!=(const Vector2& Left, const Vector2& Right) noexcept;

inline Vector2 operator-(const Vector2& In) noexcept;
inline Vector2 operator-(float Left, const Vector2& Right) noexcept;
inline Vector2 operator-(const Vector2& Left, float Right) noexcept;
inline Vector2 operator-(const Vector2& Left, const Vector2& Right) noexcept;
inline Vector2 operator+(float Left, const Vector2& Right) noexcept;
inline Vector2 operator+(const Vector2& Left, float Right) noexcept;
inline Vector2 operator+(const Vector2& Left, const Vector2& Right) noexcept;
inline Vector2 operator*(float Left, const Vector2& Right) noexcept;
inline Vector2 operator*(const Vector2& Left, float Right) noexcept;
inline Vector2 operator*(const Vector2& Left, const Vector2& Right) noexcept;
inline Vector2 operator/(float Left, const Vector2& Right) noexcept;
inline Vector2 operator/(const Vector2& Left, float Right) noexcept;
inline Vector2 operator/(const Vector2& Left, const Vector2& Right) noexcept;

inline Vector2& operator-=(Vector2& Left, float Right) noexcept;
inline Vector2& operator-=(Vector2& Left, const Vector2& Right) noexcept;
inline Vector2& operator+=(Vector2& Left, float Right) noexcept;
inline Vector2& operator+=(Vector2& Left, const Vector2& Right) noexcept;
inline Vector2& operator*=(Vector2& Left, float Right) noexcept;
inline Vector2& operator*=(Vector2& Left, const Vector2& Right) noexcept;
inline Vector2& operator/=(Vector2& Left, float Right) noexcept;
inline Vector2& operator/=(Vector2& Left, const Vector2& Right) noexcept;

//=============================================================================
// OLD
inline Vector2 Project(const Vector2& v1, const Vector2& v2);
inline Vector2 Slide(const Vector2& v, const Vector2& normal);
inline Vector2 Tangent(const Vector2& v);

inline Vector2 Bezier3(const Vector2& v0, const Vector2& v1, const Vector2& v2, float f);
inline Vector2 Bezier4(const Vector2& v0, const Vector2& v1, const Vector2& v2, const Vector2& v3, float f);
inline void Abs(Vector2& v);
inline void Floor(Vector2& v);
inline void Ceil(Vector2& v);
inline void Round(Vector2& v);

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
	constexpr Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
	constexpr Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}
	constexpr Vector3(const Vector4& v);

	constexpr Vector3& operator=(Vector3&&) = default;
	constexpr Vector3& operator=(const Vector3&) = default;

	constexpr float& operator[](size_t i) noexcept { return (&x)[i]; }
	constexpr const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector3 GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline bool Equals(const Vector3& v1, const Vector3& v2, float epsilon = EPSILON) noexcept;

inline float Distance(const Vector3& v1, const Vector3& v2);
inline float DotProduct(const Vector3& v1, const Vector3& v2);
inline Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);
inline Vector3 Reflect(const Vector3& v, const Vector3& normal);
inline Vector3 Refract(const Vector3& i, const Vector3& normal, float eta);
inline Vector3 Min(const Vector3& v1, const Vector3& v2);
inline Vector3 Max(const Vector3& v1, const Vector3& v2);
inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x);
// Spherical interpolation between two vectors
inline Vector3 SLerp(const Vector3& x, const Vector3& y, float a);
inline Vector3 Mix(const Vector3& x, const Vector3& y, float t);

inline Vector3 Rotate(const Vector3& v, float angle, const Vector3& normal);
inline Vector3 RotateX(const Vector3& v, float angle);
inline Vector3 RotateY(const Vector3& v, float angle);
inline Vector3 RotateZ(const Vector3& v, float angle);
// Rotates a 3 components vector by a quaternion.
inline Vector3 Rotate(const Quaternion& q, const Vector3& v);

// Returns the absolute angle between two vectors.
inline float Angle(const Vector3& x, const Vector3& y);

// Returns the oriented angle between two 3d vectors based from a reference axis.
inline float OrientedAngle(const Vector3& x, const Vector3& y, const Vector3& ref);

inline bool operator==(const Vector3& Left, const Vector3& Right) noexcept;
inline bool operator!=(const Vector3& Left, const Vector3& Right) noexcept;

inline Vector3 operator-(const Vector3& In) noexcept;
inline Vector3 operator-(float Left, const Vector3& Right) noexcept;
inline Vector3 operator-(const Vector3& Left, float Right) noexcept;
inline Vector3 operator-(const Vector3& Left, const Vector3& Right) noexcept;
inline Vector3 operator+(float Left, const Vector3& Right) noexcept;
inline Vector3 operator+(const Vector3& Left, float Right) noexcept;
inline Vector3 operator+(const Vector3& Left, const Vector3& Right) noexcept;
inline Vector3 operator*(float Left, const Vector3& Right) noexcept;
inline Vector3 operator*(const Vector3& Left, float Right) noexcept;
inline Vector3 operator*(const Vector3& Left, const Vector3& Right) noexcept;
inline Vector3 operator*(const Quaternion& Left, const Vector3& Right) noexcept;
inline Vector3 operator*(const Vector3& Left, const Quaternion& Right) noexcept;
inline Vector3 operator*(const Matrix3& Left, const Vector3& Right) noexcept;
inline Vector3 operator*(const Vector3& Left, const Matrix3& Right) noexcept;
inline Vector3 operator/(float Left, const Vector3& Right) noexcept;
inline Vector3 operator/(const Vector3& Left, float Right) noexcept;
inline Vector3 operator/(const Vector3& Left, const Vector3& Right) noexcept;
inline Vector3 operator/(const Matrix3& Left, const Vector3& Right) noexcept;
inline Vector3 operator/(const Vector3& Left, const Matrix3& Right) noexcept;

inline Vector3& operator-=(Vector3& Left, float Right) noexcept;
inline Vector3& operator-=(Vector3& Left, const Vector3& Right) noexcept;
inline Vector3& operator+=(Vector3& Left, float Right) noexcept;
inline Vector3& operator+=(Vector3& Left, const Vector3& Right) noexcept;
inline Vector3& operator*=(Vector3& Left, float Right) noexcept;
inline Vector3& operator*=(Vector3& Left, const Vector3& Right) noexcept;
inline Vector3& operator/=(Vector3& Left, float Right) noexcept;
inline Vector3& operator/=(Vector3& Left, const Vector3& Right) noexcept;

//=============================================================================
// OLD
inline Vector3 Project(const Vector3& v1, const Vector3& v2);
// Return the angle between this vector and another vector in degrees.
inline float Angle(const Vector3& v1, const Vector3& v2);
// Returns this vector slide along a plane defined by the given normal.
inline Vector3 Slide(const Vector3& v, const Vector3& normal);
inline Vector3 Rotate(const Vector3& v0, Vector3 ra, float angle); // TODO: удалить
inline Vector3 Bezier3(const Vector3& v0, const Vector3& v1, const Vector3& v2, float f);
inline Vector3 Bezier4(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, float f);
inline void Abs(Vector3& v);
inline void Floor(Vector3& v);
inline void Ceil(Vector3& v);
inline void Round(Vector3& v);

//=============================================================================
// Vector4
//=============================================================================
class Vector4
{
public:
	constexpr Vector4() = default;
	constexpr Vector4(Vector4&&) = default;
	constexpr Vector4(const Vector4&) = default;
	constexpr Vector4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Vector4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
	constexpr Vector4(const Vector3& v, float nw) : x(v.x), y(v.y), z(v.z), w(nw) {}

	constexpr Vector4& operator=(Vector4&&) = default;
	constexpr Vector4& operator=(const Vector4&) = default;

	constexpr float& operator[](size_t i) noexcept { return (&x)[i]; }
	constexpr const float operator[](size_t i) const noexcept { return (&x)[i]; }

	float GetLength() const;
	float GetLengthSquared() const;
	Vector4 GetNormalize() const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

inline bool Equals(const Vector4& v1, const Vector4& v2, float epsilon = EPSILON) noexcept;

inline float DotProduct(const Vector4& v1, const Vector4& v2);
inline Vector4 Min(const Vector4& v1, const Vector4& v2);
inline Vector4 Max(const Vector4& v1, const Vector4& v2);
inline Vector4 Lerp(const Vector4& a, const Vector4& b, float x);
inline Vector4 Mix(const Vector4& x, const Vector4& y, float a);

inline Vector4 Rotate(const Vector4& v, float angle, const Vector3& normal);
inline Vector4 RotateX(const Vector4& v, float angle);
inline Vector4 RotateY(const Vector4& v, float angle);
inline Vector4 RotateZ(const Vector4& v, float angle);
// Rotates a 4 components vector by a quaternion.
inline Vector4 Rotate(const Quaternion& q, const Vector4& v);

// Returns the absolute angle between two vectors.
inline float Angle(const Vector4& x, const Vector4& y);

inline bool operator==(const Vector4& Left, const Vector4& Right) noexcept;
inline bool operator!=(const Vector4& Left, const Vector4& Right) noexcept;

inline Vector4 operator-(const Vector4& In) noexcept;
inline Vector4 operator-(float Left, const Vector4& Right) noexcept;
inline Vector4 operator-(const Vector4& Left, float Right) noexcept;
inline Vector4 operator-(const Vector4& Left, const Vector4& Right) noexcept;
inline Vector4 operator+(float Left, const Vector4& Right) noexcept;
inline Vector4 operator+(const Vector4& Left, float Right) noexcept;
inline Vector4 operator+(const Vector4& Left, const Vector4& Right) noexcept;
inline Vector4 operator*(float Left, const Vector4& Right) noexcept;
inline Vector4 operator*(const Vector4& Left, float Right) noexcept;
inline Vector4 operator*(const Vector4& Left, const Vector4& Right) noexcept;
inline Vector4 operator*(const Quaternion& Left, const Vector4& Right) noexcept;
inline Vector4 operator*(const Vector4& Left, const Quaternion& Right) noexcept;
inline Vector4 operator*(const Matrix4& Left, const Vector4& Right) noexcept;
inline Vector4 operator*(const Vector4& Left, const Matrix4& Right) noexcept;
inline Vector4 operator/(float Left, const Vector4& Right) noexcept;
inline Vector4 operator/(const Vector4& Left, float Right) noexcept;
inline Vector4 operator/(const Vector4& Left, const Vector4& Right) noexcept;
inline Vector4 operator/(const Matrix4& Left, const Vector4& Right) noexcept;
inline Vector4 operator/(const Vector4& Left, const Matrix4& Right) noexcept;

inline Vector4& operator-=(Vector4& Left, float Right) noexcept;
inline Vector4& operator-=(Vector4& Left, const Vector4& Right) noexcept;
inline Vector4& operator+=(Vector4& Left, float Right) noexcept;
inline Vector4& operator+=(Vector4& Left, const Vector4& Right) noexcept;
inline Vector4& operator*=(Vector4& Left, float Right) noexcept;
inline Vector4& operator*=(Vector4& Left, const Vector4& Right) noexcept;
inline Vector4& operator/=(Vector4& Left, float Right) noexcept;
inline Vector4& operator/=(Vector4& Left, const Vector4& Right) noexcept;

//=============================================================================
// OLD
inline void Abs(Vector4& v);
inline void Floor(Vector4& v);
inline void Ceil(Vector4& v);
inline void Round(Vector4& v);

//=============================================================================
// Quaternion
//=============================================================================
class Quaternion
{
public:
	static const Quaternion Identity;

	constexpr Quaternion() = default;
	constexpr Quaternion(Quaternion&&) = default;
	constexpr Quaternion(const Quaternion&) = default;
	constexpr Quaternion(float nw, float nx, float ny, float nz) : w(nw), x(nx), y(ny), z(nz) {}
	constexpr Quaternion(float s, const Vector3& v) : w(s), x(v.x), y(v.y), z(v.z) {}
	Quaternion(const Vector3& u, const Vector3& v); // Create a quaternion from two normalized axis
	Quaternion(const Vector3& eulerAngle); // Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	Quaternion(const Matrix3& m);
	Quaternion(const Matrix4& m);

	constexpr Quaternion& operator=(Quaternion&&) = default;
	constexpr Quaternion& operator=(const Quaternion&) = default;

	constexpr float& operator[](size_t i) noexcept { return (&w)[i]; }
	constexpr const float operator[](size_t i) const noexcept { return (&w)[i]; }

	explicit operator Matrix3() const;
	explicit operator Matrix4() const;

	Quaternion Conjugate() const { return { w, -x, -y, -z }; }

	float GetLength() const;
	float GetLengthSquared() const;
	Quaternion GetNormalize() const;
	Quaternion Inverse() const;

	// returns the quaternion rotation angle.
	float GetAngle() const;
	// Returns the q rotation axis.
	Vector3 GetAxis() const;

	// Returns euler angles, pitch as x, yaw as y, roll as z.
	Vector3 EulerAngles() const;
	// Returns yaw value of euler angles expressed in radians.
	float YawAngle() const;
	// Returns pitch value of euler angles expressed in radians.
	float PitchAngle() const;
	// Returns roll value of euler angles expressed in radians.
	float RollAngle() const;

	float w = 1.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline bool Equals(const Quaternion& v1, const Quaternion& v2, float epsilon = EPSILON) noexcept;

inline Quaternion CastToQuaternion(const Matrix3& m);
inline Quaternion CastToQuaternion(const Matrix4& m);

inline float DotProduct(const Quaternion& q1, const Quaternion& q2);
inline Quaternion CrossProduct(const Quaternion& q1, const Quaternion& q2);
inline Quaternion CrossProduct(const Quaternion& q, const Vector3& v);
inline Quaternion CrossProduct(const Vector3& v, const Quaternion& q);

// Rotates a quaternion from a vector of 3 components axis and an angle.
inline Quaternion Rotate(const Quaternion& q, float angle, const Vector3& axis);
// Build a quaternion from an angle and a normalized axis.
inline Quaternion AngleAxis(float angle, const Vector3& axis);

// Build a look at quaternion based on the default handedness. Left-handed look at quaternion.
inline Quaternion QuatLookAt(const Vector3& direction, const Vector3& up);

// Spherical linear interpolation of two quaternions.
// a - Interpolation factor. The interpolation is defined beyond the range [0, 1].
// For short path spherical linear interpolation, use the SLerp function.
inline Quaternion Mix(const Quaternion& x, const Quaternion& y, float a);
// Linear interpolation of two quaternions.
// a - Interpolation factor. The interpolation is defined in the range [0, 1].
inline Quaternion Lerp(const Quaternion& x, const Quaternion& y, float a);
// Spherical linear interpolation of two quaternions.
// a - Interpolation factor. The interpolation is defined beyond the range [0, 1].
// The interpolation always take the short path and the rotation is performed at constant speed.
inline Quaternion SLerp(const Quaternion& x, const Quaternion& y, float a);
// Spherical linear interpolation of two quaternions with multiple spins over rotation axis.
// The interpolation always take the short path when the spin count is positive and long path when count is negative. Rotation is performed at constant speed.
// a - Interpolation factor. The interpolation is defined beyond the range [0, 1].
// k - Additional spin count. If Value is negative interpolation will be on "long" path.
inline Quaternion SLerp(const Quaternion& x, const Quaternion& y, float a, float k);

inline bool operator==(const Quaternion& Left, const Quaternion& Right) noexcept;
inline bool operator!=(const Quaternion& Left, const Quaternion& Right) noexcept;

inline Quaternion operator-(const Quaternion& q) noexcept;
inline Quaternion operator-(const Quaternion& Left, const Quaternion& Right) noexcept;
inline Quaternion operator+(const Quaternion& Left, const Quaternion& Right) noexcept;
inline Quaternion operator*(float Left, const Quaternion& Right) noexcept;
inline Quaternion operator*(const Quaternion& Left, float Right) noexcept;
inline Quaternion operator*(const Quaternion& Left, const Quaternion& Right) noexcept;
inline Quaternion operator/(const Quaternion& Left, float Right) noexcept;

inline Quaternion& operator-=(Quaternion& Left, const Quaternion& Right) noexcept;
inline Quaternion& operator+=(Quaternion& Left, const Quaternion& Right) noexcept;
inline Quaternion& operator*=(Quaternion& Left, float Right) noexcept;
inline Quaternion& operator*=(Quaternion& Left, const Quaternion& Right) noexcept;
inline Quaternion& operator/=(Quaternion& Left, float Right) noexcept;

//=============================================================================
// Matrix3
//=============================================================================
class Matrix3
{
public:
	static const Matrix3 Identity;

	constexpr Matrix3() = default;
	constexpr Matrix3(Matrix3&&) = default;
	constexpr Matrix3(const Matrix3&) = default;
	constexpr Matrix3(
		float x0, float y0, float z0,
		float x1, float y1, float z1,
		float x2, float y2, float z2);
	constexpr Matrix3(const Vector3& v0, const Vector3& v1, const Vector3& v2) : value{ v0, v1, v2 } {}
	Matrix3(const Matrix4& m);

	constexpr Matrix3& operator=(Matrix3&&) = default;
	constexpr Matrix3& operator=(const Matrix3&) = default;

	constexpr Vector3& operator[](size_t i) noexcept { return value[i]; }
	constexpr const Vector3& operator[](size_t i) const noexcept { return value[i]; }

	float* DataPtr() { return &(value[0].x); }
	const float* DataPtr() const { return &(value[0].x); }

	float GetDeterminant() const;
	Matrix3 Inverse() const;
	Matrix3 Transpose() const;

	Vector3 value[3] = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	};
};

inline Matrix3 CastToMatrix3(const Quaternion& q);

inline bool operator==(const Matrix3& Left, const Matrix3& Right) noexcept;
inline bool operator!=(const Matrix3& Left, const Matrix3& Right) noexcept;

inline Matrix3 operator-(const Matrix3& In) noexcept;
inline Matrix3 operator-(float Left, const Matrix3& Right) noexcept;
inline Matrix3 operator-(const Matrix3& Left, float Right) noexcept;
inline Matrix3 operator-(const Matrix3& Left, const Matrix3& Right) noexcept;
inline Matrix3 operator+(float Left, const Matrix3& Right) noexcept;
inline Matrix3 operator+(const Matrix3& Left, float Right) noexcept;
inline Matrix3 operator+(const Matrix3& Left, const Matrix3& Right) noexcept;
inline Matrix3 operator*(float Left, const Matrix3& Right) noexcept;
inline Matrix3 operator*(const Matrix3& Left, float Right) noexcept;
inline Matrix3 operator*(const Matrix3& Left, const Matrix3& Right) noexcept;
inline Matrix3 operator/(float Left, const Matrix3& Right) noexcept;
inline Matrix3 operator/(const Matrix3& Left, float Right) noexcept;
inline Matrix3 operator/(const Matrix3& Left, const Matrix3& Right) noexcept;

inline Matrix3& operator-=(Matrix3& Left, float Right) noexcept;
inline Matrix3& operator-=(Matrix3& Left, const Matrix3& Right) noexcept;
inline Matrix3& operator+=(Matrix3& Left, float Right) noexcept;
inline Matrix3& operator+=(Matrix3& Left, const Matrix3& Right) noexcept;
inline Matrix3& operator*=(Matrix3& Left, float Right) noexcept;
inline Matrix3& operator*=(Matrix3& Left, const Matrix3& Right) noexcept;
inline Matrix3& operator/=(Matrix3& Left, float Right) noexcept;
inline Matrix3& operator/=(Matrix3& Left, const Matrix3& Right) noexcept;

//=============================================================================
// Matrix4
//=============================================================================
class Matrix4
{
public:
	static const Matrix4 Zero;
	static const Matrix4 Identity;

	constexpr Matrix4() = default;
	constexpr Matrix4(Matrix4&&) = default;
	constexpr Matrix4(const Matrix4&) = default;
	constexpr Matrix4(
		float x0, float y0, float z0, float w0,
		float x1, float y1, float z1, float w1,
		float x2, float y2, float z2, float w2,
		float x3, float y3, float z3, float w3);
	constexpr Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3) : value{ v0, v1, v2, v3 } {}
	constexpr Matrix4(const Matrix3& m);

	constexpr Matrix4& operator=(Matrix4&&) = default;
	constexpr Matrix4& operator=(const Matrix4&) = default;

	constexpr Vector4& operator[](size_t i) noexcept { return value[i]; }
	constexpr const Vector4& operator[](size_t i) const noexcept { return value[i]; }

	float* DataPtr() { return &(value[0].x); }
	const float* DataPtr() const { return &(value[0].x); }

	float GetDeterminant() const;
	Matrix4 Inverse() const;
	Matrix4 Transpose() const;

	// http://www.opensource.apple.com/source/WebCore/WebCore-514/platform/graphics/transforms/TransformationMatrix.cpp
	// Decomposes the mode matrix to translations,rotation scale components
	bool Decompose(Vector3& scale, Quaternion& orientation, Vector3& translation, Vector3& skew, Vector4& perspective);

	static Matrix4 Translate(const Matrix4& m, const Vector3& v);
	static Matrix4 Rotate(const Matrix4& m, float angle, const Vector3& axis);
	static Matrix4 Scale(const Matrix4& m, const Vector3& v);
	static Matrix4 EulerAngleX(float angleX);
	static Matrix4 EulerAngleY(float angleY);
	static Matrix4 EulerAngleZ(float angleZ);
	// Creates a 3D 4 * 4 homogeneous rotation matrix from euler angles (Y * X * Z).
	static Matrix4 YawPitchRoll(float yaw, float pitch, float roll);

	// Build a rotation matrix from a normal and a up vector.
	static Matrix4 Orientation(const Vector3& normal, const Vector3& up);

	static Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 Frustum(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 Perspective(float fovy, float aspect, float zNear, float zFar);
	static Matrix4 PerspectiveFov(float fov, float width, float height, float zNear, float zFar);
	static Matrix4 InfinitePerspective(float fovy, float aspect, float zNear);
	static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	// Map the specified object coordinates into window coordinates using default near and far clip planes definition.
	// obj - Specify the object coordinates.
	// model - Specifies the current modelview matrix
	// proj - Specifies the current projection matrix
	// viewport - Specifies the current viewport
	// Return the computed window coordinates.
	// https://stackoverflow.com/questions/35261192/how-to-use-glmproject-to-get-the-coordinates-of-a-point-in-world-space
	static Vector3 Project(const Vector3& obj, const Matrix4& model, const Matrix4& proj, const Vector4& viewport);

	// Map the specified window coordinates into object coordinates using default near and far clip planes definition.
	// win - Specify the window coordinates to be mapped.
	// model Specifies the modelview matrix
	// proj - - Specifies the projection matrix
	// viewport - Specifies the viewport
	// Returns the computed object coordinates.
	static Vector3 UnProject(const Vector3& win, const Matrix4& model, const Matrix4& proj, const Vector4& viewport);

	// Define a picking region
	// center - Specify the center of a picking region in window coordinates.
	// delta - Specify the width and height, respectively, of the picking region in window coordinates.
	// viewport - Rendering viewport
	static Matrix4 PickMatrix(const Vector2& center, const Vector2& delta, const Vector4& viewport);

	Vector4 value[4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
};

inline Matrix4 CastToMatrix4(const Quaternion& q);

inline bool operator==(const Matrix4& Left, const Matrix4& Right) noexcept;
inline bool operator!=(const Matrix4& Left, const Matrix4& Right) noexcept;

inline Matrix4 operator-(const Matrix4& In) noexcept;
inline Matrix4 operator-(float Left, const Matrix4& Right) noexcept;
inline Matrix4 operator-(const Matrix4& Left, float Right) noexcept;
inline Matrix4 operator-(const Matrix4& Left, const Matrix4& Right) noexcept;
inline Matrix4 operator+(float Left, const Matrix4& Right) noexcept;
inline Matrix4 operator+(const Matrix4& Left, float Right) noexcept;
inline Matrix4 operator+(const Matrix4& Left, const Matrix4& Right) noexcept;
inline Matrix4 operator*(float Left, const Matrix4& Right) noexcept;
inline Matrix4 operator*(const Matrix4& Left, float Right) noexcept;
inline Matrix4 operator*(const Matrix4& Left, const Matrix4& Right) noexcept;
inline Matrix4 operator/(float Left, const Matrix4& Right) noexcept;
inline Matrix4 operator/(const Matrix4& Left, float Right) noexcept;
inline Matrix4 operator/(const Matrix4& Left, const Matrix4& Right) noexcept;

inline Matrix4& operator-=(Matrix4& Left, float Right) noexcept;
inline Matrix4& operator-=(Matrix4& Left, const Matrix4& Right) noexcept;
inline Matrix4& operator+=(Matrix4& Left, float Right) noexcept;
inline Matrix4& operator+=(Matrix4& Left, const Matrix4& Right) noexcept;
inline Matrix4& operator*=(Matrix4& Left, float Right) noexcept;
inline Matrix4& operator*=(Matrix4& Left, const Matrix4& Right) noexcept;
inline Matrix4& operator/=(Matrix4& Left, float Right) noexcept;
inline Matrix4& operator/=(Matrix4& Left, const Matrix4& Right) noexcept;

//=============================================================================
// Impl
//=============================================================================
#include "MicroMath.inl"
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER