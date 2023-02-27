#pragma once

#include "MicroEngine.h"
#include "PhysicsSettings.h"
#include "PhysicsBodyManager.h"

class PhysicsSystem
{
public:
	// inMaxBodies - максимальное кол-во физ. тел
	void Init(size_t inMaxBodies);
	void Close();

	void SetGravity(const Vector3& gravity);
	Vector3 GetGravity() const;

	void Update(float deltaTime);

private:
	PhysicsBodyManager m_bodyManager; // содержит все физические тела

	Vector3 m_gravity = { 0.0f, -9.81f, 0.0f };

};