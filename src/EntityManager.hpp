#pragma once

#include "Entity.hpp"

#include <vector>
#include <array>

typedef std::vector<std::shared_ptr<Entity>> EntityVector;

class EntityManager
{
	EntityVector m_entities;
	std::array<EntityVector, Tag::TAG_COUNT> m_entityMap;
	EntityVector m_toAdd;
	size_t m_total_entities = 0;

public:
	void init();
	void update();
	EntityManager();
	std::shared_ptr<Entity> addEntity(Tag tag);
	EntityVector& getEntities();
	EntityVector& getEntities(Tag tag);
};