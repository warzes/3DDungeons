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
	m_viewMatrix = { 
		x.x,                             y.x,                             z.x,                            0.0f, 
		x.y,                             y.y,                             z.y,                            0.0f, 
		x.z,                             y.z,                             z.z,                            0.0f, 
		-DotProduct(x, position), -DotProduct(y, position), -DotProduct(z, position), 1.0f };
}
//-----------------------------------------------------------------------------
bool FlyingCamera::OnKeys(short Keys, float FrameTime, Vector3& Movement)
{
	float Speed = m_speed;
	if (Keys & CAMERA_KEY_SHIFT) Speed *= 2.0f;
	if (Keys & CAMERA_KEY_CONTROL) Speed *= 0.5f;

	const float Distance = Speed * FrameTime;

	Vector3 Up = y * Distance;
	Vector3 Right = x * Distance;
	Vector3 Forward = -z * Distance;

	if (Keys & CAMERA_KEY_W) Movement += Forward;
	if (Keys & CAMERA_KEY_S) Movement -= Forward;
	if (Keys & CAMERA_KEY_A) Movement -= Right;
	if (Keys & CAMERA_KEY_D) Movement += Right;
	if (Keys & CAMERA_KEY_R) Movement += Up;
	if (Keys & CAMERA_KEY_F) Movement -= Up;

	return Keys & 0x3F ? true : false;
}
//-----------------------------------------------------------------------------
void FlyingCamera::OnMouseMove(int dx, int dy)
{
	if (dx != 0)
	{
		const float DeltaX = (float)dx * m_sensitivity;

		x = Rotate(x, DeltaX, Vector3::Up);
		y = Rotate(y, DeltaX, Vector3::Up);
		z = Rotate(z, DeltaX, Vector3::Up);
	}

	if (dy != 0)
	{
		const float DeltaY = (float)dy * m_sensitivity;

		y = Rotate(y, DeltaY, x);
		z = Rotate(z, DeltaY, x);

		if (y.y < 0.0f)
		{
			z = Vector3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			y = CrossProduct(z, x);
		}
	}

	calculateViewMatrix();
}
//-----------------------------------------------------------------------------