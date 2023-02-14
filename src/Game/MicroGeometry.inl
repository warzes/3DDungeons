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