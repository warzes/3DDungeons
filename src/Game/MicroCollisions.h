#pragma once

//=============================================================================
// Header
//=============================================================================
#include "MicroMath.h"

//=============================================================================
// Closest
//=============================================================================

// Finds the closest point to the source point on the given line segment
inline Vector2 ClosestPointOnLineSegment(
	const Vector2& a,     // point one of line segment
	const Vector2& b,     // point two of line segment
	const Vector2& point  // source point
);
inline Vector3 ClosestPointOnLineSegment(
	const Vector3& a,     // point one of line segment
	const Vector3& b,     // point two of line segment
	const Vector3& point  // source point
);

//=============================================================================
// Check
//=============================================================================

inline bool CheckPointInTriangle(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, const Vector3& point);

//=============================================================================
// Impl
//=============================================================================
#include "MicroCollisions.inl"