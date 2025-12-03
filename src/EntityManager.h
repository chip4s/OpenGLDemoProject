#ifndef ENTITY_MANAGER_CLASS_H
#define ENTITY_MANAGER_CLASS_H

#include "Entity.h"
#include <memory>



//list of all components so get all entities by comp. can be contiguous
using ComponentVectors = std::tuple
<
	std::vector<CTransform>,
	std::vector<CMesh>,
	std::vector<CPointLight>,
	std::vector<CDirectionalLight>,
	std::vector<CSpotLight>,
	std::vector<CTexture>
>;

class EntityManager
{
private:

	//following vectors should not change in size after Entity Manager constructor is called

	//total list of all components
	ComponentVectors m_AllComponents;

	//list of entities
	std::vector<Entity> m_Entities;

	//returns index into entity vector that contains the first entity where .alive == false
	int FindFirstEmptyEntity();

public:
	//constructor
	//max entities is just default reserve (can NOT expand later to hold more entities)
	EntityManager(int maxEntities);

	//returns list of all comps. of type T
	template<typename T>
	std::vector<T>& GetComponentsByType()
	{
		//gets vector of type T from tuple
		return std::get<std::vector<T>>(m_AllComponents);
	}

	//Adds entity to m_Entities
	Entity& AddEntity(const std::string& tag);

	//sets entity.exists to false and all comp.exists to false
	void DeleteEntity(Entity& entityToDelete);

	//adds comp. of type T to entity
	template<typename T, typename... Targ>
	T& AddComponent(Entity& entityToAdd, Targ&&... componentArguments)
	{
		//get vector of components and using entity.index get component
		auto& component = GetComponentsByType<T>().at(entityToAdd.m_index);

		//create component
		component = T(std::forward<Targ>(componentArguments)...);

		//set component.exists to true
		component.exists = true;

		//return reference to component
		return component;
	}

	//gets comp. from entity
	template<typename T>
	T& GetComponentByEntity(const Entity& entityToGetFrom)
	{
		return std::get<std::vector<T>>(m_AllComponents).at(entityToGetFrom.m_index);
	}

};

#endif