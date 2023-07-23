#pragma once

#include "EntityManager.hpp"

EntityManager::EntityManager() : m_total_entities(0) 
{
	m_entities.clear();
	m_toAdd.clear();
};

void EntityManager::init() {}

void EntityManager::update()
{
	// update entity vector if there's element in toAdd
	for (auto e : m_toAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}
	m_toAdd.clear();

	// erase entity in entity vector if entity is not active
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e)
		{ return e->isActive() == false; }
	), m_entities.end());

	// Iterate through the map and erase elements from the vector when a specific condition is fulfilled
	for (auto& vector : m_entityMap) {
		vector.erase(std::remove_if(vector.begin(), vector.end(), [](std::shared_ptr<Entity> e)
			{ return e->isActive() == false; }
		), vector.end());
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(Tag tag)
{
	// to make entity that has private constructor
	std::shared_ptr<Entity> e = std::shared_ptr<Entity>(new Entity(tag, m_total_entities++));
	m_toAdd.push_back(e);
	return e;
};

EntityVector& EntityManager::getEntities()
{
	return m_entities;
}

EntityVector& EntityManager::getEntities(Tag tag)
{
	return m_entityMap[tag];
}
