#pragma once

#include "Components.hpp"

#include <memory>
#include <tuple>

class EntityManager;

typedef std::tuple<
	CTransform,
	CLifespan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState
> ComponentTuple;

enum Tag { DefaultTag = -1, Player, Enemy, Bullet, Tile, Dec, TAG_COUNT = Dec + 1 };

class Entity
{
	// make constructor private so entity cannot be created directly
	friend class EntityManager;

	size_t m_id = 0;
	Tag m_tag = DefaultTag;
	bool m_active = true;
	ComponentTuple m_components;

	Entity(const Tag tag, size_t id);

public:
	// private member access
	void destroy();
	bool isActive() const;
	const Tag getTag() const;
	const size_t getId() const;

	// component function
	template <typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template<typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T>
	void removeComponent()
	{
		auto& component = getComponent<T>();
		component = T();
		component.has = false;
	}
};
