#include "PhysicsBodyManager.h"

void PhysicsBodyManager::Init(size_t inMaxBodies)
{
	m_bodies.reserve(inMaxBodies);
}

void PhysicsBodyManager::Close()
{
}