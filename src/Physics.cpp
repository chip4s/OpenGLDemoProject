#include "Physics.h"


void Physics::HandleRigidBodies(EntityManager& entityManager, float dT)
{
	// get list of rigid bodies and transforms
	auto& allRigidBodies = entityManager.GetComponentsByType<CRigidBody>();
	auto& allTransforms = entityManager.GetComponentsByType<CTransform>();

	//iterate through this list
	for (int i = 0; i < allRigidBodies.size(); i++)
	{
		//cache current rigid body and transform
		CRigidBody& rigidBody = allRigidBodies[i];
		CTransform& transform = allTransforms[i];

		//skip an iteration if either comp. does not exist
		if (rigidBody.exists == false || transform.exists == false)
		{
			continue;
		}


		
		if (rigidBody.dynamic == true)
		{
			//std::cout << glm::to_string(rigidBody.acceleration) << " is acceleration\n\n";
			//std::cout << glm::to_string(rigidBody.velocity) << " is velocity\n\n";
			
			//apply gravity
			rigidBody.netForce.y = rigidBody.mass * rigidBody.gravity;
			

			//net Force / mass = acceleration
			rigidBody.acceleration = (rigidBody.netForce / rigidBody.mass) * dT;//later could set mass to inverse mass to use * not /


			//add acceleration to velocity
			rigidBody.velocity += rigidBody.acceleration * dT;

			//add velocity to position
			transform.position += rigidBody.velocity * dT;
		}
	}
}

void Physics::CheckBoxCollisions(EntityManager& entityManager, float dT)
{
	//get all box collider comp.s
	auto& allBoxColliders = entityManager.GetComponentsByType<CBoxCollider>();
	
	//get all transform comp.s
	auto& allTransforms = entityManager.GetComponentsByType<CTransform>();

	//get all rigid body comp.s
	auto& allRigidBodies = entityManager.GetComponentsByType<CRigidBody>();

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
				//if (!allRigidBodies[i].dynamic || !allRigidBodies[j].dynamic)
					//continue;

				//Collision resolution

				//find mag. needed to move in the y-axis to stop colliding
				float yChange = 0.0f;
				
				if (transformOne.position.y >= transformTwo.position.y)
				{
					//objOne is above objTwo
					yChange = std::abs(maxYTwo) - std::abs(minYOne);
				}
				else
				{
					//objOne is below objTwo
					yChange = std::abs(maxYOne) - std::abs(minYTwo);
				}
				
				
				//find mag. needed to move in the x-axis stop colliding
				float xChange = 0.0f;

				if (transformOne.position.x >= transformTwo.position.x)
				{
					//objOne is to the right of objTwo
					xChange = std::abs(maxXTwo) - std::abs(minXOne);
				}
				else
				{
					//objOne is to the left of objTwo
					xChange = std::abs(maxXOne) - std::abs(minXTwo);
				}


				//find mag. needed to move in the x-axis stop colliding
				float zChange = 0.0f;

				if (transformOne.position.z >= transformTwo.position.z)
				{
					//objOne is in front of objTwo
					zChange = std::abs(maxZTwo) - std::abs(minZOne);
				}
				else
				{
					//objOne is behind objTwo
					zChange = std::abs(maxZOne) - std::abs(minZTwo);
				}

				
				float posDifferrenceY = std::abs(transformOne.position.y - transformTwo.position.y) - boxColliderOne.height * 0.5f - boxColliderTwo.height * 0.5f;
				float posDifferrenceX = std::abs(transformOne.position.x - transformTwo.position.x) - boxColliderOne.width * 0.5f - boxColliderTwo.width * 0.5f;
				float posDifferrenceZ = std::abs(transformOne.position.z - transformTwo.position.z) - boxColliderOne.length * 0.5f - boxColliderTwo.length * 0.5f;


				CRigidBody& rbOne = allRigidBodies[i];
				CRigidBody& rbTwo = allRigidBodies[j];

				if (posDifferrenceY > posDifferrenceX && posDifferrenceY > posDifferrenceZ)
				{
					//std::cout << "y collision\n";
					if (rbOne.dynamic == rbTwo.dynamic) //collision between two moving objects/static objects
					{
						transformOne.position.y += yChange * 0.5f;
						transformTwo.position.y += -yChange * 0.5f;
					}
					else if (rbOne.dynamic == true) //collision between one moving and one static object
					{
						transformOne.position.y += yChange;

						rbOne.netForce.y = 0.0f;
						rbOne.velocity.y = 0.0f;
					}
					else //one static and one moving (reversed)
					{
						transformTwo.position.y += -yChange;
					}
				}
				if (posDifferrenceX > posDifferrenceY && posDifferrenceX > posDifferrenceZ)
				{
					if (rbOne.dynamic == rbTwo.dynamic) //collision between two moving objects/static objects
					{
						transformOne.position.x += xChange * 0.5f;
						transformTwo.position.x += -xChange * 0.5f;
					}
					else if (rbOne.dynamic == true) //collision between one moving and one static object
					{
						transformOne.position.x += xChange;
					}
					else //one static and one moving (reversed)
					{
						transformTwo.position.x += -xChange;
					}
				}
				if (posDifferrenceZ > posDifferrenceX && posDifferrenceZ > posDifferrenceY)
				{
					if (rbOne.dynamic == rbTwo.dynamic) //collision between two moving objects/static objects
					{
						transformOne.position.z += zChange * 0.5f;
						transformTwo.position.z += -zChange * 0.5f;
					}
					else if (rbOne.dynamic == true)//collision between one moving and one static object
					{
						transformOne.position.z += zChange;
					}
					else //one static and one moving (reversed)
					{
						transformTwo.position.z += -zChange;
					}
				}
				//std::cout << std::abs(xChange) << " : xChange\n";
				//std::cout << std::abs(yChange) << " : yChange\n";
				//std::cout << std::abs(zChange) << " : zChange\n";
				
			}
		}
	}
}
