#include "EntityManager.h"
#include <algorithm>
#include <type_traits>

//constructor
EntityManager::EntityManager(int maxEntities)//reserves space in all vectors for a given amount of entities
{
	//code below reserves every vector in the tuple by max entities
	std::apply([maxEntities](auto&&... componentVector) {(componentVector.resize(maxEntities), ...);}, m_AllComponents);

	//reserves entity vector
	m_Entities.resize(maxEntities);
}

int EntityManager::FindFirstEmptyEntity()
{
	//loop through Entities and finds first entity where .alive == false, then returns its index
	//could use std::find_if, but I think return an index is simpler than an iterator
	for (int i = 0;i < m_Entities.size();i++)
	{
		if (m_Entities.at(i).m_alive == false)
		{
			return i;
		}
	}

	//if code above hasn't found an empty spot means
	//we are trying to add past the max limit
	std::cout << "Exceeded entity limit\n";
	assert(0 == 0);
	return -1;

}

//Adds entity to m_Entities
Entity& EntityManager::AddEntity(const std::string& tag)
{
	//handles adding entity
	//first find empty location to have a new entity
	int entityIndex = FindFirstEmptyEntity();

	//creates entity at first empty location
	m_Entities.at(entityIndex) = Entity(entityIndex, tag);

	//then default construct all of its comp.s
	std::apply([entityIndex](auto&&... componentVector) {((componentVector.at(entityIndex) = {}), ...);}, m_AllComponents);

	//return reference to entity
	return m_Entities.at(entityIndex);

}

//sets entity.exists to false and all comp.exists to false
void EntityManager::DeleteEntity(Entity& entityToDelete)
{
	//set m_alive to false in vector (by reference)
	entityToDelete.m_alive = false;

	//gets index into comp. vectors from entity
	size_t index = entityToDelete.m_index;
	//set all comps.exists to false
	std::apply([index](auto&&... componentVector) {((componentVector.at(index).exists = false), ...);}, m_AllComponents);
}
