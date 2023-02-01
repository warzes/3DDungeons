#pragma once

//=============================================================================
// Header
//=============================================================================

#include "MicroMath.h"

//=============================================================================
// Camera
//=============================================================================
#define CAMERA_KEY_W 0x0001
#define CAMERA_KEY_S 0x0002
#define CAMERA_KEY_A 0x0004
#define CAMERA_KEY_D 0x0008
#define CAMERA_KEY_R 0x0010
#define CAMERA_KEY_F 0x0020
#define CAMERA_KEY_Q 0x0040
#define CAMERA_KEY_E 0x0080
#define CAMERA_KEY_C 0x0100
#define CAMERA_KEY_SPACE 0x0200
#define CAMERA_KEY_SHIFT 0x0400
#define CAMERA_KEY_CONTROL 0x0800

class Object;

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual bool CheckCollisions(Object* Objects, int ObjectsCount, Vector3& Movement, int Depth = 0) { return false; }
	virtual void Look(const Vector3& Position, const Vector3& Reference);
	virtual void Move(const Vector3& Movement);
	virtual bool OnKeys(short Keys, float FrameTime, Vector3& Movement) = 0;
	virtual void OnMouseMove(int dx, int dy) = 0;
	virtual void OnMouseWheel(short zDelta) {}

	const Matrix4& GetViewMatrix() const { return m_viewMatrix; }

	Vector3 x = Vector3::Right;
	Vector3 y = Vector3::Up;
	Vector3 z = Vector3::Forward;
	Vector3 position = { 0.0f };



protected:
	virtual void calculateViewMatrix();

	Matrix4 m_viewMatrix;
	Matrix4 m_viewMatrixInverse;

	float m_speed = 2.5f;
	float m_sensitivity = 0.025f;
};

class FlyingCamera : public ICamera
{
public:
	bool OnKeys(short Keys, float FrameTime, Vector3& Movement);
	void OnMouseMove(int dx, int dy);
};