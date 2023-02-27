#pragma once

#include "MicroMath.h"

// Intersection test result.
enum Intersection
{
	OUTSIDE = 0,
	INTERSECTS,
	INSIDE
};

//=============================================================================
// Rect
//=============================================================================

// Two-dimensional bounding rectangle.
class Rect
{
public:
	/// Rect in the range (-1, -1) - (1, 1)
	static const Rect Full;
	/// Rect in the range (0, 0) - (1, 1)
	static const Rect Positive;
	static const Rect Zero;

	Rect() = default;
	Rect(Rect&&) = default;
	Rect(const Rect&) = default;
	Rect(const Vector2& min_, const Vector2& max_) : min(min_), max(max_) {}
	Rect(float left, float top, float right, float bottom) : min(left, top), max(right, bottom) {}
	Rect(const Vector4& vector) : min(vector.x, vector.y), max(vector.z, vector.w) {}

	Rect& operator=(Rect&&) = default;
	Rect& operator=(const Rect&) = default;

	bool operator==(const Rect& rhs) const { return min == rhs.min && max == rhs.max; }
	bool operator!=(const Rect& rhs) const { return !(*this == rhs); }

	void Set(const Rect& rect);
	void Set(const Vector2& min_, const Vector2& max_);
	void Set(const Vector2& point);

	void Merge(const Vector2& point);
	void Merge(const Rect& rect);

	void Undefine();

	void Clip(const Rect& rect);

	Vector4 ToVector4() const { return { min.x, min.y, max.x, max.y }; }
	bool IsDefined() const { return (min.x <= max.x); }
	Vector2 Center() const { return (max + min) * 0.5f; }
	Vector2 Size() const { return max - min; }
	Vector2 HalfSize() const { return (max - min) * 0.5f; }
	const void* Data() const { return &min.x; }

	Intersection IsInside(const Vector2& point) const;

	Vector2 min = { INFINITY };
	Vector2 max = { -INFINITY };
};

//=============================================================================
// Triangle
//=============================================================================
class Triangle
{
public:
	Triangle() = default;
	Triangle(Triangle&&) = default;
	Triangle(const Triangle&) = default;
	Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3) : v{ v1, v2, v3 } {}

	Triangle& operator=(Triangle&&) = default;
	Triangle& operator=(const Triangle&) = default;

	constexpr Vector3& operator[](size_t i) noexcept { return v[i]; }
	constexpr const Vector3& operator[](size_t i) const noexcept { return v[i]; }

	Vector3 GetCentroid() const; // Get center of triangle

	// Vertices
	Vector3 v[3];
};

//=============================================================================
// Sphere
//=============================================================================
class Sphere
{
public:
	Sphere() = default;
	Sphere(const Vector3& point, float _radius) : position(point), radius(_radius) {}

	Vector3 position;
	float radius = 0.0f;
};

//=============================================================================
// AABB
//=============================================================================
class AABB
{
public:
	AABB() = default;
	AABB(AABB&&) = default;
	AABB(const AABB&) = default;
	AABB(const Vector3& inMin, const Vector3& inMax) : min(inMin), max(inMax) {}
	AABB(const Vector3& inCenter, float inRadius) : min(inCenter - inRadius), max(inCenter + inRadius) {}

	AABB& operator=(AABB&&) = default;
	AABB& operator=(const AABB&) = default;

	void AddPoint(const Vector3& point); // Add point in bounding box
	void AddAABB(const AABB& rhs); // Add bounding box in bounding box
	void AddTriangle(const Triangle& tri); // Encapsulate triangle in bounding box

	Vector3 GetCenter() const;
	Vector3 GetExtent() const; // Get extent of bounding box (half of the size)
	Vector3 GetSize() const;
	float GetSurfaceArea() const; // Get surface area of bounding box
	float GetVolume() const;

	bool Contains(const Vector3& point) const;
	bool Contains(const AABB& rhs) const;
	bool Overlaps(const AABB& aabb) const; // Check if this box overlaps with another box

	AABB Intersection(const AABB& rhs) const; // Intersect this bounding box with inOther, returns the intersection

	void Translate(const Vector3& v);
	AABB Transformed(const Matrix4& matrix) const;
	AABB Scaled(const Vector3& scale) const;// Scale this bounding box, can handle non-uniform and negative scaling

	static AABB FromTwoPoints(const Vector3& point1, const Vector3& point2); // Create box from 2 points

	Vector3 min;
	Vector3 max;
};

inline Vector3 GetClosestPoint(const AABB& aabb, const Vector3& inPoint); // Get the closest point on or in this box to inPoint
inline float GetSqDistanceTo(const AABB& aabb, const Vector3& inPoint); // Get the squared distance between inPoint and this box (will be 0 if in Point is inside the box)

inline bool operator==(const AABB& Left, const AABB& Right) noexcept;
inline bool operator!=(const AABB& Left, const AABB& Right) noexcept;

//=============================================================================
// Frustum
//=============================================================================


//=============================================================================
// Plane
//=============================================================================
class Plane
{
public:
	Plane() = default;
	Plane(const Vector3& origin, const Vector3& normal);
	// Construct from triangle
	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);

	bool IsFrontFacingTo(const Vector3& direction) const;
	float SignedDistanceTo(const Vector3& point) const;

	Vector4 equation;
	Vector3 origin;
	Vector3 normal;
};

#include "MicroGeometry.inl"