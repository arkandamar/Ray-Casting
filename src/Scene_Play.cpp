#pragma once

#include "Scene_Play.hpp"
#include "GameEngine.hpp"

#include <iostream>

using std::cout;
using std::endl;

Scene_Play::Scene_Play(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}

void Scene_Play::init()
{
	registerAction(sf::Keyboard::A, Left);
	registerAction(sf::Keyboard::W, Up);
	registerAction(sf::Keyboard::S, Down);
	registerAction(sf::Keyboard::D, Right);
	registerAction(sf::Keyboard::P, Paused);

	spawnPlayer();
	spawnBuild();
}

void Scene_Play::doAction(const Action& action)
{
	sDoAction(action);
}

void Scene_Play::sDoAction(const Action& action)
{
	if (action.actionType() == KeyPress)
	{
		switch (action.actionName())
		{
		case Left: m_player->getComponent<CInput>().left = true; break;
		case Up: m_player->getComponent<CInput>().up = true; break;
		case Down: m_player->getComponent<CInput>().down = true; break;
		case Right: m_player->getComponent<CInput>().right = true; break;
		case Paused: m_paused ? m_paused = false : m_paused = true; break;
		default:
			break;
		}
	}

	if (action.actionType() == KeyRelease)
	{
		switch (action.actionName())
		{
		case Left: m_player->getComponent<CInput>().left = false; break;
		case Up: m_player->getComponent<CInput>().up = false; break;
		case Down: m_player->getComponent<CInput>().down = false; break;
		case Right: m_player->getComponent<CInput>().right = false; break;
		default:
			break;
		}
	}
}

void Scene_Play::onEnd()
{
	m_hasEnded = true;
	m_game->quit();
}

void Scene_Play::update()
{
	// update entity manager
	m_entityManager.update();

	if (!m_paused)
	{
		sLifespan();
		sCollision();
		sMovement();
	}
	sAnimation();
	sRender();

	// update current frame
	m_currentFrame++;
}

void Scene_Play::spawnPlayer()
{
	auto p = m_entityManager.addEntity(Player);
	p->addComponent<CInput>();
	p->addComponent<CTransform>(Vec2((float) width() / 2, (float) height() / 2));
	p->addComponent<CShape>((float) 20, 30, sf::Color(255, 0, 0));
	m_player = p;
}

void Scene_Play::spawnBuild()
{

}

void Scene_Play::sLifespan()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CLifespan>())
		{
			if (e->getComponent<CLifespan>().remaining <= 0)
			{
				e->destroy();
			}
			e->getComponent<CLifespan>().remaining--;
		}
	}
}

void Scene_Play::sCollision()
{

}

void Scene_Play::sAnimation()
{

}

void Scene_Play::sMovement()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CTransform>())
		{
			if (e->hasComponent<CShape>())
			{
				e->getComponent<CShape>().circle.setPosition
				(
					e->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x,
					e->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y
				);
			}

			if (e->hasComponent<CConvex>())
			{
				e->getComponent<CConvex>().convex.setPosition
				(
					e->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x,
					e->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y
				);
			}
		}
	}

	if (m_player->hasComponent<CInput>())
	{
		if (m_player->getComponent<CInput>().up)
		{
			m_player->getComponent<CTransform>().pos.y -= 2;
		}

		if (m_player->getComponent<CInput>().down)
		{
			m_player->getComponent<CTransform>().pos.y += 2;
		}

		if (m_player->getComponent<CInput>().left)
		{
			m_player->getComponent<CTransform>().pos.x -= 2;
		}

		if (m_player->getComponent<CInput>().right)
		{
			m_player->getComponent<CTransform>().pos.x += 2;
		}
	}
}

void Scene_Play::sRender()
{
	m_game->window().clear();
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CShape>())
		{
			m_game->window().draw(m_player->getComponent<CShape>().circle);
		}

		if (e->hasComponent<CConvex>())
		{
			m_game->window().draw(m_player->getComponent<CConvex>().convex);
		}
	}
	m_game->window().display();
}