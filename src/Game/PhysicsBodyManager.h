#pragma once

#include "RigidBody.h"

// информация о текущих физических телах
struct PhysicsBodyManagerStats
{

};

class PhysicsBodyManager
{
public:
	void Init(size_t inMaxBodies);
	void Close();

private:
	// массив всех RigidBody. 
	// Массив выделяется нужного размера, поэтому нет реллокаций (возможно избавится от вектора и сделать массивом)
	std::vector<RigidBody*> m_bodies;

	// текущее кол-во тел
	size_t m_numBodies = 0;
};