#ifndef PHYSICS_H
#define PHYSICS_H
#define GLM_ENABLE_EXPERIMENTAL

#include "EntityManager.h"


class Physics
{
private:
	float gravity = -98.0f;
public:
	Physics() {};

	void HandleRigidBodies(EntityManager& entityManager, float dT);
	void CheckBoxCollisions(EntityManager& entityManager, float dT);

};


#endif