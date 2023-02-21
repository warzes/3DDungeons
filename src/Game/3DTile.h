#pragma once

#include "MicroEngine.h"

namespace Tile3DManager
{
	bool Create();
	void Destroy();

	void BeginDraw(const Matrix4& proj, const Matrix4& view);
	void DrawWall(const Vector3& position);
	void DrawFloor(const Vector3& position);
	void DrawCeil(const Vector3& position);
}