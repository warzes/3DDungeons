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