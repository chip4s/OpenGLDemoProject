#ifndef PHYSICS_H
#define PHYSICS_H
#define GLM_ENABLE_EXPERIMENTAL

#include "EntityManager.h"


class Physics
{
public:
	Physics() {};

	void CheckBoxCollisions(EntityManager& entityManager);

};


#endif