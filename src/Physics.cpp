#include "Physics.h"


void Physics::CheckBoxCollisions(EntityManager& entityManager)
{
	//get all box collider comp.s
	auto& allBoxColliders = entityManager.GetComponentsByType<CBoxCollider>();
	
	//get all transform comp.s
	auto& allTransforms = entityManager.GetComponentsByType<CTransform>();

	for (int i = 0; i < allBoxColliders.size(); i++)
	{
		//cache both comp.s and skip a loop forward if one comp. doesn't exist
		CBoxCollider& boxColliderOne = allBoxColliders[i];
		CTransform& transformOne = allTransforms[i];
		if (boxColliderOne.exists == false || transformOne.exists == false)
		{
			continue;
		}

		
		//next loop over all box colliders again
		for (int j = 0; j < allBoxColliders.size(); j++)
		{
			//if both entities are same skip this iteration
			if (i == j)
			{
				continue;
			}

			
			//cache new box collider and its transform also checking if it exists
			CBoxCollider& boxColliderTwo = allBoxColliders[j];
			CTransform& transformTwo = allTransforms[j];
			if (boxColliderTwo.exists == false || transformTwo.exists == false)
			{
				continue;
			}

			
			//now check for collision using AABB
			bool collidingX = false;
			bool collidingY = false;
			bool collidingZ = false;

			//check x-axis for collisions
			//store max and mins of first object on x-axis
			float maxXOne = transformOne.position.x + boxColliderOne.width * 0.5f;
			float minXOne = transformOne.position.x - boxColliderOne.width * 0.5f;

			//store max and mins of second object on x-axis
			float maxXTwo = transformTwo.position.x + boxColliderTwo.width * 0.5f;
			float minXTwo = transformTwo.position.x - boxColliderTwo.width * 0.5f;

			//check collision on x-axis using max and mins
			if(maxXOne >= minXTwo && maxXTwo >= minXOne)
			{
				collidingX = true;
			}


			//check y-axis for collisions
			//store max and mins of first object on y-axis
			float maxYOne = transformOne.position.y + boxColliderOne.height * 0.5f;
			float minYOne = transformOne.position.y - boxColliderOne.height * 0.5f;

			//store max and mins of second object on y-axis
			float maxYTwo = transformTwo.position.y + boxColliderTwo.height * 0.5f;
			float minYTwo = transformTwo.position.y - boxColliderTwo.height * 0.5f;

			//check collision on y-axis using max and mins
			if(maxYOne >= minYTwo && maxYTwo >= minYOne)
			{
				collidingY = true;
			}


			//check z-axis for collisions
			//store max and mins of first object on z-axis
			float maxZOne = transformOne.position.z + boxColliderOne.length * 0.5f;
			float minZOne = transformOne.position.z - boxColliderOne.length * 0.5f;

			//store max and mins of second object on z-axis
			float maxZTwo = transformTwo.position.z + boxColliderTwo.length * 0.5f;
			float minZTwo = transformTwo.position.z - boxColliderTwo.length * 0.5f;

			//check collision on z-axis using max and mins
			if(maxZOne >= minZTwo && maxZTwo >= minZOne)
			{
				collidingZ = true;
			}
			

			//check collision on all axis
			if (collidingX && collidingY && collidingZ)
			{
				std::cout << "is colliding\n\n";
			}
		}
	}
}
