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
// AABB
//=============================================================================

inline void AABB::Merge(const AABB& rhs)
{
	AddPoint(rhs.min);
	AddPoint(rhs.max);
}

inline void AABB::AddPoint(const Vector3& point)
{
	min = MinCoords(point, min);
	max = MaxCoords(point, max);
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

inline void AABB::ToTransform(const Matrix4& matrix)
{
	Vector3 points[8];
	points[0] = min;
	points[7] = max;
	points[1] = Vector3(points[0].x, points[0].y, points[7].z);
	points[2] = Vector3(points[0].x, points[7].y, points[0].z);
	points[3] = Vector3(points[0].x, points[7].y, points[7].z);
	points[4] = Vector3(points[7].x, points[0].y, points[0].z);
	points[5] = Vector3(points[7].x, points[0].y, points[7].z);
	points[6] = Vector3(points[7].x, points[7].y, points[0].z);

	for( int j = 0; j < 8; ++j )
	{
		points[j] = matrix.TransformPoint(points[j]);
	}

	Vector3 new_min = points[0];
	Vector3 new_max = points[0];

	for( int j = 0; j < 8; ++j )
	{
		new_min = MinCoords(points[j], new_min);
		new_max = MaxCoords(points[j], new_max);
	}

	min = new_min;
	max = new_max;
}

inline void AABB::Translate(const Vector3& v)
{
	min += v;
	max += v;
}

inline void AABB::GetCorners(const Transform& tr, Vector3* points) const
{
	Vector3 p(min.x, min.y, min.z);
	points[0] = tr.ToTransform(p);
	p = Vector3(min.x, min.y, max.z);
	points[1] = tr.ToTransform(p);
	p = Vector3(min.x, max.y, min.z);
	points[2] = tr.ToTransform(p);
	p = Vector3(min.x, max.y, max.z);
	points[3] = tr.ToTransform(p);
	p = Vector3(max.x, min.y, min.z);
	points[4] = tr.ToTransform(p);
	p = Vector3(max.x, min.y, max.z);
	points[5] = tr.ToTransform(p);
	p = Vector3(max.x, max.y, min.z);
	points[6] = tr.ToTransform(p);
	p = Vector3(max.x, max.y, max.z);
	points[7] = tr.ToTransform(p);
}

inline void AABB::GetCorners(const Matrix4& matrix, Vector3* points) const
{
	Vector3 p(min.x, min.y, min.z);
	points[0] = matrix.TransformPoint(p);
	p = Vector3(min.x, min.y, max.z);
	points[1] = matrix.TransformPoint(p);
	p = Vector3(min.x, max.y, min.z);
	points[2] = matrix.TransformPoint(p);
	p = Vector3(min.x, max.y, max.z);
	points[3] = matrix.TransformPoint(p);
	p = Vector3(max.x, min.y, min.z);
	points[4] = matrix.TransformPoint(p);
	p = Vector3(max.x, min.y, max.z);
	points[5] = matrix.TransformPoint(p);
	p = Vector3(max.x, max.y, min.z);
	points[6] = matrix.TransformPoint(p);
	p = Vector3(max.x, max.y, max.z);
	points[7] = matrix.TransformPoint(p);
}

inline Vector3 AABB::MinCoords(const Vector3& a, const Vector3& b)
{
	return Vector3(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
}

inline Vector3 AABB::MaxCoords(const Vector3& a, const Vector3& b)
{
	return Vector3(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
}

inline void AABB::Shrink(float x)
{
	min += x;
	max -= x;
}

inline AABB AABB::Intersection(const AABB& rhs) const
{
	return AABB(Max(rhs.min, min), Min(rhs.max, max));
}


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