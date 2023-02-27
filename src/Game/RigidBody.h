#pragma once

#include "MicroEngine.h"

class RigidBody
{
	friend class PhysicsBodyManager;
public:
	RigidBody() = default;
	RigidBody(const RigidBody&) = delete;
	~RigidBody() = default;

	void operator=(const RigidBody&) = delete;

private:
	Vector3 m_position;    // World space position of center of mass
	Quaternion m_rotation; // World space rotation of center of mass
	AABB m_bounds;         // World space bounding box of the body
};