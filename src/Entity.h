#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "Components.h"

class EntityManager;

class Entity
{
	friend class EntityManager;// entity manager has access to Entity private
private:

	// if it doesn't exist, can overwrite data with another entity
	bool m_alive = false;

	// index into all comp. vectors
	size_t m_index = 0;

	// entity tag
	std::string m_entityTag = "default";

	// private constructor so only entity manager can create entities
	Entity(size_t id, const std::string& tag)
		: m_index(id),
		m_entityTag(tag),
		m_alive(true) {};
	// exists will default be true and only on deletion of entity be false (when constructed with parameters)

public:

	//defaults constructor for resize()
	Entity() {};
	
};

#endif