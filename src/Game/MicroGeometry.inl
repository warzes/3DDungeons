#pragma once

//=============================================================================
// Rect
//=============================================================================

inline const Rect Rect::Full(-1.0f, -1.0f, 1.0f, 1.0f);
inline const Rect Rect::Positive(0.0f, 0.0f, 1.0f, 1.0f);
inline const Rect Rect::Zero(0.0f, 0.0f, 0.0f, 0.0f);

inline void Rect::Set(const Rect& rect)
{
	min = rect.min;
	max = rect.max;
}

inline void Rect::Set(const Vector2& min_, const Vector2& max_)
{
	min = min_;
	max = max_;
}

inline void Rect::Set(const Vector2& point)
{
	min = max = point;
}

inline void Rect::Merge(const Vector2& point)
{
	// If undefined, set initial dimensions
	if( !IsDefined() )
	{
		min = max = point;
		return;
	}

	if( point.x < min.x )
		min.x = point.x;
	if( point.x > max.x )
		max.x = point.x;
	if( point.y < min.y )
		min.y = point.y;
	if( point.y > max.y )
		max.y = point.y;
}

inline void Rect::Merge(const Rect& rect)
{
	if( min.x > max.x )
	{
		min = rect.min;
		max = rect.max;
		return;
	}

	if( rect.min.x < min.x )
		min.x = rect.min.x;
	if( rect.min.y < min.y )
		min.y = rect.min.y;
	if( rect.max.x > max.x )
		max.x = rect.max.x;
	if( rect.max.y > max.y )
		max.y = rect.max.y;
}

inline void Rect::Undefine()
{
	min = Vector2(INFINITY, INFINITY);
	max = -min;
}

inline void Rect::Clip(const Rect& rect)
{
	if( rect.min.x > min.x )
		min.x = rect.min.x;
	if( rect.max.x < max.x )
		max.x = rect.max.x;
	if( rect.min.y > min.y )
		min.y = rect.min.y;
	if( rect.max.y < max.y )
		max.y = rect.max.y;

	if( min.x > max.x )
		std::swap(min.x, max.x);
	if( min.y > max.y )
		std::swap(min.y, max.y);
}

inline Intersection Rect::IsInside(const Vector2 & point) const
{
	if( point.x < min.x || point.y < min.y || point.x > max.x || point.y > max.y )
		return OUTSIDE;
	else
		return INSIDE;
}

//=============================================================================
// Triangle
//=============================================================================
inline Vector3 Triangle::GetCentroid() const
{
	return (v[0] + v[1] + v[2]) * (1.0f / 3.0f);
}

//=============================================================================
// AABB
//=============================================================================

inline void AABB::AddPoint(const Vector3& point)
{
	min = Min(min, point);
	max = Max(max, point);
}

inline void AABB::AddAABB(const AABB& rhs)
{
	min = Min(min, rhs.min);
	max = Max(max, rhs.max);
}

inline void AABB::AddTriangle(const Triangle& tri)
{
	AddPoint(tri[0]);
	AddPoint(tri[1]);
	AddPoint(tri[2]);
}

inline Vector3 AABB::GetCenter() const
{
	return 0.5f * (min + max);
}

inline Vector3 AABB::GetExtent() const
{
	return 0.5f * (max - min);
}

inline Vector3 AABB::GetSize() const
{
	return max - min;
}

inline float AABB::GetSurfaceArea() const
{
	Vector3 extent = max - min;
	return 2.0f * (extent.x * extent.y + extent.x * extent.z + extent.y * extent.z);
}

inline float AABB::GetVolume() const
{
	Vector3 extent = max - min;
	return extent.x * extent.y * extent.z;
}

inline bool AABB::Contains(const Vector3& point) const
{
	if( min.x > point.x ) return false;
	if( min.y > point.y ) return false;
	if( min.z > point.z ) return false;
	if( point.x > max.x ) return false;
	if( point.y > max.y ) return false;
	if( point.z > max.z ) return false;
	return true;
}

inline bool AABB::Contains(const AABB& rhs) const
{
	// TODO: проверить
	if( min.x > rhs.min.x ) return false;
	if( min.y > rhs.min.y ) return false;
	if( min.z > rhs.min.z ) return false;
	if( rhs.min.x > max.x ) return false;
	if( rhs.min.y > max.y ) return false;
	if( rhs.min.z > max.z ) return false;
	return true;
}

inline bool AABB::Overlaps(const AABB& aabb) const
{
	if( min.x > aabb.max.x ) return false;
	if( min.y > aabb.max.y ) return false;
	if( min.z > aabb.max.z ) return false;
	if( aabb.min.x > max.x ) return false;
	if( aabb.min.y > max.y ) return false;
	if( aabb.min.z > max.z ) return false;
	return true;
}

inline AABB AABB::Intersection(const AABB& rhs) const
{
	return { Max(rhs.min, min), Min(rhs.max, max) };
}

inline void AABB::Translate(const Vector3& v)
{
	min += v;
	max += v;
}

inline AABB AABB::Transformed(const Matrix4& inMatrix) const
{
	// Start with the translation of the matrix
	Vector3 newMin = inMatrix.GetTranslation();
	Vector3 newMax = inMatrix.GetTranslation();

	// Now find the extreme points by considering the product of the min and max with each column of inMatrix
	for( int c = 0; c < 3; ++c )
	{
		Vector3 col = inMatrix[c];
		Vector3 a = col * min[c];
		Vector3 b = col * max[c];

		newMin += Min(a, b);
		newMax += Max(a, b);
	}
	// Return the new bounding box
	return { newMin, newMax };
}

inline AABB AABB::Scaled(const Vector3& scale) const
{
	return AABB::FromTwoPoints(min * scale, max * scale);
}

inline AABB AABB::FromTwoPoints(const Vector3& point1, const Vector3& point2)
{
	return AABB(Min(point1, point2), Max(point1, point2));
}

inline Vector3 GetClosestPoint(const AABB& aabb, const Vector3& inPoint)
{
	return Min(Max(inPoint, aabb.min), aabb.max);
}

inline float GetSqDistanceTo(const AABB& aabb, const Vector3& inPoint)
{
	return (GetClosestPoint(aabb, inPoint) - inPoint).GetLengthSquared();
}

inline bool operator==(const AABB& Left, const AABB& Right) noexcept { return Left.min == Right.min && Left.max == Right.max; }
inline bool operator!=(const AABB& Left, const AABB& Right) noexcept { return Left.min != Right.min ||Left.max != Right.max; }

//=============================================================================
// Plane
//=============================================================================

inline Plane::Plane(const Vector3& Origin, const Vector3& Normal)
{
	origin = Origin;
	normal = Normal;
	equation = { normal, -DotProduct(origin, normal) };
}

inline Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	normal = CrossProduct(p2 - p1, p3 - p1).GetNormalize();
	origin = p1;
	equation = { normal, -DotProduct(origin, normal) };
}

inline float Plane::SignedDistanceTo(const Vector3& point) const
{
	return (DotProduct(point, normal)) + equation.w;
}

inline bool Plane::IsFrontFacingTo(const Vector3& direction) const
{
	const float dot = DotProduct(normal, direction);
	return (dot <= 0.0f);
}