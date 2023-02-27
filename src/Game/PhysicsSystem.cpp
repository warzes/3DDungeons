#include "PhysicsSystem.h"

void PhysicsSystem::Init(size_t inMaxBodies)
{
	m_bodyManager.Init(inMaxBodies);
}

void PhysicsSystem::Close()
{
	m_bodyManager.Close();
}

void PhysicsSystem::SetGravity(const Vector3& gravity)
{
	m_gravity = gravity;
}

Vector3 PhysicsSystem::GetGravity() const
{
	return m_gravity;
}

void PhysicsSystem::Update(float deltaTime)
{
}