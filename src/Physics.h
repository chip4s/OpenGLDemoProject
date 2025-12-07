#ifndef PHYSICS_H
#define PHYSICS_H
#define GLM_ENABLE_EXPERIMENTAL

#include "EntityManager.h"


class Physics
{
private:
	
public:
	Physics() {};
	int accuracy = 1;
	void HandleRigidBodies(EntityManager& entityManager, float dT);
	void CheckBoxCollisions(EntityManager& entityManager, float dT);

};


#endif