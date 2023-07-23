#pragma once

#include "Entity.hpp"

Entity::Entity(const Tag tag, size_t id) 
	: m_tag(tag), m_id(id),
	m_components(CTransform(), CLifespan(), CInput(), CBoundingBox(), CAnimation(), CGravity(), CState()) {};

void Entity::destroy()
{
	m_active = false;
}

bool Entity::isActive() const
{
	return m_active;
}

const Tag Entity::getTag() const
{
	return m_tag;
}

const size_t Entity::getId() const
{
	return m_id;
}