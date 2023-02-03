#pragma once

#include "MicroMath.h"

//=============================================================================
// Point2
//=============================================================================

// finds the closest point to the source point on the given line segment
inline constexpr Vector3 ClosestPointOnLineSegment(
	const Vector3& a,     // point one of line segment
	const Vector3& b,     // point two of line segment
	const Vector3& point  // source point
);