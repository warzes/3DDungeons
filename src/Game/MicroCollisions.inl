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

//=============================================================================
// 
//=============================================================================

inline bool CheckPointInTriangle(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, const Vector3& point)
{
#if 0
	const Vector3 u = tri1 - tri0;
	const Vector3 v = tri2 - tri0;
	const Vector3 w = point - tri0;

	const Vector3 vw = CrossProduct(v, w);
	const Vector3 vu = CrossProduct(v, u);
	if( DotProduct(vw, vu) < 0.0f )
		return false;

	const Vector3 uw = CrossProduct(u, w);
	const Vector3 uv = CrossProduct(u, v);
	if( DotProduct(uw, uv) < 0.0f )
		return false;

	const float d = uv.GetLength();
	const float r = vw.GetLength() / d;
	const float t = uw.GetLength() / d;
	return ((r + t) <= 1.0f);
#else
	// barycentric()
	// returns the barycentric coordinates of point p within triangle t0 - t1 - t2 with the result packed into a vec (u = x, v = y, w = z)
	const Vector3 v0 = tri1 - tri0;
	const Vector3 v1 = tri2 - tri0;
	const Vector3 v2 = point - tri0;

	const float d00 = DotProduct(v0, v0);
	const float d01 = DotProduct(v0, v1);
	const float d11 = DotProduct(v1, v1);
	const float d20 = DotProduct(v2, v0);
	const float d21 = DotProduct(v2, v1);
	const float denom = d00 * d11 - d01 * d01;

	const float v = (d11 * d20 - d01 * d21) / denom; // y
	const float w = (d00 * d21 - d01 * d20) / denom; // z
	const float u = 1.0f - v - w; // x
	// barycentric

	return u >= 0.0f && v >= 0.0f && w >= 0.0f;
#endif
}