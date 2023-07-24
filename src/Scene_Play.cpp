#pragma once

#include "Scene_Play.hpp"
#include "GameEngine.hpp"
#include "Utils.hpp"

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

	if (m_linesSegment.size() == 0)
	{
		registerLine();
	}

	if (!m_paused)
	{
		sLifespan();
		sIntercept();
		sMovement();
	}
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
	auto fir = m_entityManager.addEntity(Build);
	fir->addComponent<CTransform>(Vec2(0, 0));
	fir->addComponent<CConvex>(3, sf::Color(0, 0, 0, 0));
	fir->getComponent<CConvex>().convex.setPoint(0, sf::Vector2f(176, 230));
	fir->getComponent<CConvex>().convex.setPoint(1, sf::Vector2f(35, 510));
	fir->getComponent<CConvex>().convex.setPoint(2, sf::Vector2f(334, 418));
	fir->getComponent<CConvex>().convex.setOutlineColor(sf::Color(255, 123, 0));
	fir->getComponent<CConvex>().convex.setOutlineThickness(2);

	auto sec = m_entityManager.addEntity(Build);
	sec->addComponent<CConvex>(4, sf::Color(0, 0, 0, 0));
	sec->getComponent<CConvex>().convex.setPoint(0, sf::Vector2f(380, 80));
	sec->getComponent<CConvex>().convex.setPoint(1, sf::Vector2f(300, 180));
	sec->getComponent<CConvex>().convex.setPoint(2, sf::Vector2f(455, 200));
	sec->getComponent<CConvex>().convex.setPoint(3, sf::Vector2f(620, 100));
	sec->getComponent<CConvex>().convex.setOutlineColor(sf::Color(255, 123, 0));
	sec->getComponent<CConvex>().convex.setOutlineThickness(2);

	auto thr = m_entityManager.addEntity(Build);
	thr->addComponent<CConvex>(5, sf::Color(0, 0, 0, 0));
	thr->getComponent<CConvex>().convex.setPoint(0, sf::Vector2f(700, 365));
	thr->getComponent<CConvex>().convex.setPoint(1, sf::Vector2f(763, 497));
	thr->getComponent<CConvex>().convex.setPoint(2, sf::Vector2f(867, 585));
	thr->getComponent<CConvex>().convex.setPoint(3, sf::Vector2f(965, 455));
	thr->getComponent<CConvex>().convex.setPoint(4, sf::Vector2f(929, 263));
	thr->getComponent<CConvex>().convex.setOutlineColor(sf::Color(255, 123, 0));
	thr->getComponent<CConvex>().convex.setOutlineThickness(2);
}

void Scene_Play::registerLine()
{
	for (auto& e : m_entityManager.getEntities(Build))
	{
		size_t pointCount = e->getComponent<CConvex>().convex.getPointCount();
		for (int i = 0; i < pointCount; i++)
		{
			float diffX = 0.0f, diffY = 0.0f;
			if (i == pointCount - 1)
			{
				diffX = e->getComponent<CConvex>().convex.getPoint(i).x - e->getComponent<CConvex>().convex.getPoint(0).x;
				diffY = e->getComponent<CConvex>().convex.getPoint(i).y - e->getComponent<CConvex>().convex.getPoint(0).y;
			}
			else
			{
				diffX = e->getComponent<CConvex>().convex.getPoint(i).x - e->getComponent<CConvex>().convex.getPoint((size_t) i + 1).x;
				diffY = e->getComponent<CConvex>().convex.getPoint(i).y - e->getComponent<CConvex>().convex.getPoint((size_t) i + 1).y;
			}
			m_linesSegment.push_back(Vec2(diffX, diffY));
		}
	}
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

void Scene_Play::sRayCasting()
{
	// init for new angle
	m_angles.clear();

	// init for new points
	m_points.clear();

	// register points from convex shape
	for (auto& e : m_entityManager.getEntities(Build))
	{
		for (int i = 0; i < e->getComponent<CConvex>().convex.getPointCount(); i++)
		{
			float x = e->getComponent<CConvex>().convex.getPoint(i).x;
			float y = e->getComponent<CConvex>().convex.getPoint(i).y;
			m_points.push_back(Vec2(x, y));
		}
	}

	// calculate angle between player and points
	for (auto& p : m_points)
	{
		// calculate points's angle compare to player pos
		Vec2 pPos = m_player->getComponent<CTransform>().pos;
		Vec2 diff = p - pPos;
		float angle = acosf(diff.x / diff.dist());

		// all point that above m_player will get negative angle
		if (p.y - pPos.y < 0) angle *= -1;
		m_angles.push_back(angle);

		// make deviation of rays based on angles
		float diffX, diffY, deviation = 0.01f;
		Vec2 newPoint;

		// slightly right
		float angleRight = angle - deviation;
		diffX = cos(angleRight) * diff.dist();
		diffY = sin(angleRight) * diff.dist();
		newPoint = Vec2(pPos.x + diffX, pPos.y + diffY);
		m_points.push_back(newPoint);

		// slightly left
		float angleLeft = angle + deviation;
		diffX = cos(angleLeft) * diff.dist();
		diffY = sin(angleLeft) * diff.dist();
		newPoint = Vec2(pPos.x + diffX, pPos.y + diffY);
		m_points.push_back(newPoint);
	}

	for (auto& p : m_points)
	{
		drawLine(m_player->getComponent<CTransform>().pos, p);
	}
}

void Scene_Play::sIntercept()
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
	// render ray from m_player
	sRayCasting();

	// render entity
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CShape>())
		{
			m_game->window().draw(e->getComponent<CShape>().circle);
		}

		if (e->hasComponent<CConvex>())
		{
			m_game->window().draw(e->getComponent<CConvex>().convex);
		}
	}

	// render points
	for (auto& p : m_points)
	{
		sf::CircleShape point(5);
		point.setOrigin(point.getRadius(), point.getRadius());
		point.setFillColor(sf::Color::Red);
		point.setPosition(p.x, p.y);
		m_game->window().draw(point);
	}

	// render intercept
	for (auto& i : m_intercepts)
	{
		sf::CircleShape point(5);
		point.setOrigin(point.getRadius(), point.getRadius());
		point.setFillColor(sf::Color::Red);
		point.setPosition(i.x, i.y);
		m_game->window().draw(point);
	}

	m_game->window().display();
}