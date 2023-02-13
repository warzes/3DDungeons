#pragma once

//=============================================================================
// Closest point
//=============================================================================
inline Vector2 ClosestPointOnLineSegment(const Vector2& a, const Vector2& b, const Vector2& point)
{
	const float LineLength = Distance(a, b);
	Vector2 Vector = point - a;
	Vector2 LineDirection = (b - a) / LineLength;

	// Project Vector to LineDirection to get the distance of point from a
	const float Distance = DotProduct(Vector, LineDirection);

	if( Distance <= 0.0f ) return a;
	if( Distance >= LineLength ) return b;
	return a + LineDirection * Distance;
}

inline Vector3 ClosestPointOnLineSegment(const Vector3& a, const Vector3& b, const Vector3& point)
{
	const float LineLength = Distance(a, b);
	Vector3 Vector = point - a;
	Vector3 LineDirection = (b - a) / LineLength;

	// Project Vector to LineDirection to get the distance of point from a
	const float Distance = DotProduct(Vector, LineDirection);

	if( Distance <= 0.0f ) return a;
	if( Distance >= LineLength ) return b;
	return a + LineDirection * Distance;
}