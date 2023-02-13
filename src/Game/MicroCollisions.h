#pragma once

//=============================================================================
// Header
//=============================================================================
#include "MicroMath.h"

//=============================================================================
// Closest point
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
// Impl
//=============================================================================
#include "MicroCollisions.inl"