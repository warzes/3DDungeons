#include "MicroGraphics.h"

//=============================================================================
// Camera
//=============================================================================
//-----------------------------------------------------------------------------
void ICamera::Look(const Vector3& Position, const Vector3& Reference)
{
	position = Position;
	z = (Position - Reference).Normalize();
	x = CrossProduct(Vector3::Up, z).Normalize();
	y = CrossProduct(z, x);

	calculateViewMatrix();
}
//-----------------------------------------------------------------------------
void ICamera::Move(const Vector3& Movement)
{
	position += Movement;
	calculateViewMatrix();
}
//-----------------------------------------------------------------------------
void ICamera::calculateViewMatrix()
{
	m_viewMatrix = { x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -DotProduct(x, position), -DotProduct(y, position), -DotProduct(z, position), 1.0f };
	m_viewMatrixInverse = m_viewMatrix.Inverse();
}
//-----------------------------------------------------------------------------