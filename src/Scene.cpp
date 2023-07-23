#pragma once

#include "Scene.hpp"
#include "GameEngine.hpp"

// private member function
void Scene::setPaused(bool paused)
{
	m_paused = paused;
}

// public member function
Scene::Scene() {}

Scene::Scene(GameEngine* gameEngine) : m_game(gameEngine) {}

void Scene::simulate(const size_t frames)
{
	// simulate things
}

void Scene::registerAction(int inputKey, EnumAction actionName)
{
	// register input key -> action in m_actionMap
	m_actionMap[inputKey] = actionName;
}

size_t Scene::width() const
{
	// return window width
	return m_game->window().getSize().x;
}

size_t Scene::height() const
{
	// return window height
	return m_game->window().getSize().y;
}

size_t Scene::currentFrame() const
{
	return m_currentFrame;
}

bool Scene::hasEnded() const
{
	return m_hasEnded;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
	// draw gird line
	sf::Vertex line[2];
	line[0].position = sf::Vector2f(p1.x, p1.y);
	line[0].color = sf::Color::White;
	line[1].position = sf::Vector2f(p2.x, p2.y);
	line[1].color = sf::Color::White;
	m_game->window().draw(line, 2, sf::Lines);
}