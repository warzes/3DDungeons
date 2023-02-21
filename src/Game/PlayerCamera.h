#pragma once

#include "MicroEngine.h"

namespace PlayerCamera
{
	void SetPosition(const Vector3& pos, const Vector3& eyeTarget);

	void Update(bool FreeCameraRotate, bool FreeCameraMove);

	Matrix4 GetView();
}