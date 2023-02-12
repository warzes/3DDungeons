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
constexpr const float    EPSILON = 1.0e-6f;
constexpr const float    PI = 3.14159265358979323846f;
constexpr const float    HALF_PI = PI * 0.5f;
constexpr const float    DEG2RAD = PI / 180.0f;
constexpr const float    RAD2DEG = 180.0f / PI;

class Color;
class Point2;
class Vector2;
class Vector3;
class Vector4;
class Quaternion;
class Matrix3;
class Matrix4;
class Matrix3_old;
class Matrix4_old;

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
inline constexpr float Lerp(float a, float b, float f) noexcept; // Linear interpolation between two float values.
inline constexpr float Mix(float u, float v, float a) noexcept;

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
inline Color operator-(const Color& Left, const Color& Right) noexcept;
inline Color operator+(const Color& Left, const Color& Right) noexcept;
inline Color operator*(const Color& Left, const Color& Right) noexcept;
inline Color& operator-=(Color& Left, const Color& Right) noexcept;
inline Color& operator+=(Color& Left, const Color& Right) noexcept;
inline Color& operator*=(Color& Left, const Color& Right) noexcept;

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

inline Vector2 Min(const Vector2& v1, const Vector2& v2);
inline Vector2 Max(const Vector2& v1, const Vector2& v2);
inline Vector2 Lerp(const Vector2& a, const Vector2& b, float x);
inline Vector2 Mix(const Vector2& u, const Vector2& v, float a);


inline Vector2 Project(const Vector2& v1, const Vector2& v2);
inline Vector2 Slide(const Vector2& v, const Vector2& normal);
inline Vector2 Tangent(const Vector2& v);
inline Vector2 Rotate(const Vector2& v, float angle);
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
inline Vector3 Min(const Vector3& v1, const Vector3& v2);
inline Vector3 Max(const Vector3& v1, const Vector3& v2);
inline Vector3 Lerp(const Vector3& a, const Vector3& b, float x);
inline Vector3 Mix(const Vector3& u, const Vector3& v, float t);

inline Vector3 Project(const Vector3& v1, const Vector3& v2);
inline Vector3 Rotate(const Vector3& u, float angle, const Vector3& v); // TODO: удалить?
// Return the angle between this vector and another vector in degrees.
inline float Angle(const Vector3& v1, const Vector3& v2);
// Returns this vector slide along a plane defined by the given normal.
inline Vector3 Slide(const Vector3& v, const Vector3& normal);
inline Vector3 Rotate(const Vector3& v0, Vector3 ra, float angle);
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

	float* operator&() { return (float*)this; }

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

inline Vector4 Min(const Vector4& v1, const Vector4& v2);
inline Vector4 Max(const Vector4& v1, const Vector4& v2);
inline Vector4 Lerp(const Vector4& a, const Vector4& b, float x);
inline Vector4 Mix(const Vector4& u, const Vector4& v, float a);

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

	float GetLength() const;
	float GetLengthSquared() const;

	float w = 1.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;




	// OLD
	//Quaternion(float angle, const Vector3& axis); // Construct from an angle and axis.


	Matrix3 ToMatrix3() const;
	Matrix4 ToMatrix4() const;

	// Define from an angle and axis.
	void FromAngleAxis(float angle, const Vector3& axis); // TODO: проверить
	void FromMatrix(const Matrix4_old& m0);

	Quaternion Conjugate() const { return { -x, -y, -z, w }; }

	Quaternion GetNormalize() const;
	Quaternion Inverse() const;

	// returns the quaternion rotation angle.
	float GetAngle() const;
	// Returns the q rotation axis.
	Vector3 GetAxis() const;

	// Return Euler angles
	Vector3 EulerAngles() const;
	// Return yaw angle
	float YawAngle() const;
	// Return pitch angle
	float PitchAngle() const;
	// Return roll angle
	float RollAngle() const;
};

inline bool Equals(const Quaternion& v1, const Quaternion& v2, float epsilon = EPSILON) noexcept;

inline Quaternion CastToQuaternion(const Matrix3& m);
inline Quaternion CastToQuaternion(const Matrix4& m);

inline float DotProduct(const Quaternion& q1, const Quaternion& q2);

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
// OLD

// Spherical linear interpolation of two quaternions.
// a - Interpolation factor. The interpolation is defined beyond the range [0, 1].
inline Quaternion Mix(const Quaternion& x, const Quaternion& y, float a);
// Linear interpolation of two quaternions.
inline Quaternion Lerp(const Quaternion& x, const Quaternion& y, float a);
// Spherical linear interpolation of two quaternions.
inline Quaternion SLerp(const Quaternion& x, const Quaternion& y, float a);

inline Quaternion CrossProduct(const Quaternion& q1, const Quaternion& q2);



//=============================================================================
// Matrix3
//=============================================================================
class Matrix3
{
public:
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

	float GetDeterminant() const;
	Matrix4 Inverse() const;
	Matrix4 Transpose() const;

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
// Matrix3 old
//=============================================================================
class Matrix3_old
{
public:
	constexpr Matrix3_old() = default;
	constexpr Matrix3_old(Matrix3_old&&) = default;
	constexpr Matrix3_old(const Matrix3_old&) = default;
	constexpr Matrix3_old(
		float x0, float y0, float z0,
		float x1, float y1, float z1,
		float x2, float y2, float z2);
	Matrix3_old(const Matrix4_old& m);

	constexpr Matrix3_old& operator=(Matrix3_old&&) = default;
	constexpr Matrix3_old& operator=(const Matrix3_old&) = default;

	constexpr float& operator[](size_t i) noexcept { return m[i]; }
	constexpr const float operator[](size_t i) const noexcept { return m[i]; }

	constexpr void Set(
		float x0, float y0, float z0,
		float x1, float y1, float z1,
		float x2, float y2, float z2);
	constexpr void Set(const Matrix3_old& M);

	float GetDeterminant() const;

	Matrix3_old Transpose() const;
	Matrix3_old Inverse() const;

	void Scale(const Vector3& scale);

	float m[9] = {  1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f };
};

inline Matrix3_old Matrix3Scale(const Vector3& scale);
inline Matrix3_old Matrix3RotationX(float angle);
inline Matrix3_old Matrix3RotationY(float angle);
inline Matrix3_old Matrix3RotationZ(float angle);
inline Matrix3_old Matrix3Rotation(const Quaternion& q);

inline bool operator==(const Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline bool operator!=(const Matrix3_old& Left, const Matrix3_old& Right) noexcept;

inline Matrix3_old operator-(const Matrix3_old& In) noexcept;
inline Matrix3_old operator-(float Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator-(const Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old operator-(const Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator+(float Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator+(const Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old operator+(const Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator*(float Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator*(const Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old operator*(const Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator/(float Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old operator/(const Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old operator/(const Matrix3_old& Left, const Matrix3_old& Right) noexcept;

inline Matrix3_old& operator-=(Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old& operator-=(Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old& operator+=(Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old& operator+=(Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old& operator*=(Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old& operator*=(Matrix3_old& Left, const Matrix3_old& Right) noexcept;
inline Matrix3_old& operator/=(Matrix3_old& Left, float Right) noexcept;
inline Matrix3_old& operator/=(Matrix3_old& Left, const Matrix3_old& Right) noexcept;

//=============================================================================
// Matrix4 old
//=============================================================================
class Matrix4_old
{
public:
	constexpr Matrix4_old() = default;
	constexpr Matrix4_old(Matrix4_old&&) = default;
	constexpr Matrix4_old(const Matrix4_old&) = default;
	constexpr Matrix4_old(
		float x0, float y0, float z0, float w0,
		float x1, float y1, float z1, float w1,
		float x2, float y2, float z2, float w2,
		float x3, float y3, float z3, float w3);
	constexpr Matrix4_old(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3);
	constexpr Matrix4_old(const Matrix3_old& m);

	constexpr Matrix4_old& operator=(Matrix4_old&&) = default;
	constexpr Matrix4_old& operator=(const Matrix4_old&) = default;

	constexpr float& operator[](size_t i) noexcept { return m[i]; }
	constexpr const float operator[](size_t i) const noexcept { return m[i]; }

	constexpr void Set(
		float x0, float y0, float z0, float w0,
		float x1, float y1, float z1, float w1,
		float x2, float y2, float z2, float w2,
		float x3, float y3, float z3, float w3);
	constexpr void Set(const Matrix4_old& M);

	void Scale(const Vector3& scale);
	void Translate(const Vector3& pos);

	float GetDeterminant() const;

	Matrix4_old Transpose() const;
	Matrix4_old Inverse() const;

	float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
};

inline Matrix4_old Cofactor(const Matrix3_old&m);
inline Matrix4_old Matrix4Scale(const Vector3& scale);
inline Matrix4_old Matrix4RotationX(float angle);
inline Matrix4_old Matrix4RotationY(float angle);
inline Matrix4_old Matrix4RotationZ(float angle);
inline Matrix4_old Matrix4Rotation(const Vector3& axis, float angle);
inline Matrix4_old Matrix4Rotation(const Quaternion& q);
inline Matrix4_old Matrix4Translate(const Vector3& v);

inline Matrix4_old LookAt(const Vector3& eye, const Vector3& dir, const Vector3& up); // левосторонее
inline Matrix4_old Ortho(float left, float right, float bottom, float top, float n, float f);
inline Matrix4_old Perspective(float fov_y, float aspect, float zNear, float zFar); // левосторонее


inline bool operator==(const Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline bool operator!=(const Matrix4_old& Left, const Matrix4_old& Right) noexcept;

inline Matrix4_old operator-(const Matrix4_old& In) noexcept;
inline Matrix4_old operator-(float Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator-(const Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old operator-(const Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator+(float Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator+(const Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old operator+(const Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator*(float Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator*(const Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old operator*(const Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator/(float Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old operator/(const Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old operator/(const Matrix4_old& Left, const Matrix4_old& Right) noexcept;

inline Matrix4_old& operator-=(Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old& operator-=(Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old& operator+=(Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old& operator+=(Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old& operator*=(Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old& operator*=(Matrix4_old& Left, const Matrix4_old& Right) noexcept;
inline Matrix4_old& operator/=(Matrix4_old& Left, float Right) noexcept;
inline Matrix4_old& operator/=(Matrix4_old& Left, const Matrix4_old& Right) noexcept;

//=============================================================================
// Impl
//=============================================================================
#include "MicroMath.inl"
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER