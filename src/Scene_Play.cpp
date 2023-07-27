#pragma once

#include "Scene_Play.hpp"
#include "GameEngine.hpp"
#include "Utils.hpp"

#include <iostream>
#include <set>

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
		sRayCasting();
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
	p->addComponent<CShape>((float) 6, 30, sf::Color(255, 0, 0));
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
		for (size_t i = 0; i < pointCount; i++)
		{
			LineSegment line;
			if (i == pointCount - 1)
			{
				line.subs = Vec2
				(
					e->getComponent<CConvex>().convex.getPoint(0).x - e->getComponent<CConvex>().convex.getPoint(i).x,
					e->getComponent<CConvex>().convex.getPoint(0).y - e->getComponent<CConvex>().convex.getPoint(i).y
				);
				line.tail = Vec2(e->getComponent<CConvex>().convex.getPoint(i).x, e->getComponent<CConvex>().convex.getPoint(i).y);
				line.head = Vec2(e->getComponent<CConvex>().convex.getPoint(0).x, e->getComponent<CConvex>().convex.getPoint(0).y);
			}
			else
			{
				line.subs = Vec2
				(
					e->getComponent<CConvex>().convex.getPoint(i + 1).x - e->getComponent<CConvex>().convex.getPoint(i).x,
					e->getComponent<CConvex>().convex.getPoint(i + 1).y - e->getComponent<CConvex>().convex.getPoint(i).y
				);
				line.tail = Vec2(e->getComponent<CConvex>().convex.getPoint(i).x, e->getComponent<CConvex>().convex.getPoint(i).y);
				line.head = Vec2(e->getComponent<CConvex>().convex.getPoint(i + 1).x, e->getComponent<CConvex>().convex.getPoint(i + 1).y);
			}
			m_linesSegment.push_back(line);
		}
	}

	// register line on window edge
	LineSegment left = { NULL, Vec2(0,0), Vec2(0, height()), Vec2(0, height()) };
	m_linesSegment.push_back(left);
	LineSegment bottom = { NULL, Vec2(0, height()), Vec2(width(), height()), Vec2(width(), 0)};
	m_linesSegment.push_back(bottom);
	LineSegment right = { NULL, Vec2(width(), height()), Vec2(width(), 0), Vec2(0, -(float) height()) };
	m_linesSegment.push_back(right);
	LineSegment upper = { NULL, Vec2(width(), 0), Vec2(0, 0), Vec2(-(float) width(), -(float) height())};
	m_linesSegment.push_back(upper);
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

void Scene_Play::sPoint()
{
	//// init for new angle
	//m_angles.clear();

	//// calculate angle between player and points
	//for (auto& p : m_points)
	//{
	//	// calculate points's angle compare to player pos
	//	Vec2 pPos = m_player->getComponent<CTransform>().pos;
	//	Vec2 diff = p - pPos;
	//	float angle = acosf(diff.x / diff.dist());

	//	// all point that above m_player will get negative angle
	//	if (p.y - pPos.y < 0) angle *= -1;
	//	m_angles.push_back(angle);

	//	// make deviation of rays based on angles
	//	float diffX, diffY, deviation = 0.01f;
	//	Vec2 newPoint;

	//	// slightly right
	//	float angleRight = angle - deviation;
	//	diffX = cos(angleRight) * diff.dist();
	//	diffY = sin(angleRight) * diff.dist();
	//	newPoint = Vec2(pPos.x + diffX, pPos.y + diffY);
	//	m_points.push_back(newPoint);

	//	// slightly left
	//	float angleLeft = angle + deviation;
	//	diffX = cos(angleLeft) * diff.dist();
	//	diffY = sin(angleLeft) * diff.dist();
	//	newPoint = Vec2(pPos.x + diffX, pPos.y + diffY);
	//	m_points.push_back(newPoint);
	//}
}

void Scene_Play::sRayCasting()
{
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

	m_rays.clear();
	size_t id = 0;
	for (auto& p : m_points)
	{
		m_rays.push_back(LineSegment{id++, m_player->getComponent<CTransform>().pos, p, p - m_player->getComponent<CTransform>().pos });
	}
}

void Scene_Play::sIntercept()
{
	m_intercepts.clear();

	std::map<size_t, std::vector<Intersect>> tempInterselect;

	for (auto& r : m_rays)
	{
		for (auto& l : m_linesSegment)
		{
			Intersect inter = LineIntersect(r, l);

			// if intercept occurred, push intersect to temporary array
			if (inter.result)
			{
				tempInterselect[r.id].push_back(inter);
			}
		}
	}

	// checking invalid intersection
	for (auto& inter : tempInterselect)
	{
		// check if there're rays that make more than 1 intersection
		auto& arrInter = inter.second;

		if (inter.second.size() > 1)
		{
			// compare all intersection with same rays and delete the greater value until only one left
			float threshold = 1;
			while (arrInter.size() > 1)
			{
				for (auto it = arrInter.begin(); it != arrInter.end();) {
					if ((*it).scalar > threshold) {
						it = arrInter.erase(it); // Erase the element and get the next valid iterator
					}
					else {
						threshold = (*it).scalar;
						++it; // Move to the next element
					}
				}
			}
			m_intercepts.push_back(arrInter.at(0).pos);
		}
		else
		{
			m_intercepts.push_back(inter.second.at(0).pos);
		}
	}

	for (auto& r : m_rays)
	{
		// check if ray in list of intersection
		bool isRayInList = false;
		for (auto it = tempInterselect.begin(); it != tempInterselect.end(); it++)
		{
			if (it->first == r.id) isRayInList = true;
		}

		// if none then make ray as an intercept
		if (!isRayInList)
		{
			m_intercepts.push_back(r.head);
		}
	}
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

	// render points
	for (auto& p : m_points)
	{
		sf::CircleShape point(5);
		point.setOrigin(point.getRadius(), point.getRadius());
		point.setFillColor(sf::Color::Red);
		point.setPosition(p.x, p.y);
		m_game->window().draw(point);
	}

	// render intercepts point
	for (auto& i : m_intercepts)
	{
		// draw line between player to intercept
		drawLine(m_player->getComponent<CTransform>().pos, i);

		sf::CircleShape point(5);
		point.setOrigin(point.getRadius(), point.getRadius());
		point.setFillColor(sf::Color::Red);
		point.setPosition(i.x, i.y);
		m_game->window().draw(point);
	}

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

	m_game->window().display();
}

Intersect Scene_Play::LineIntersect(LineSegment r, LineSegment s) const
{
	// make necessary property
	Intersect inter = { false, Vec2(0,0)};

	float T1, T2;

	// Solve for T2!
	T2 = (r.subs.x * (s.tail.y - r.tail.y) + r.subs.y * (r.tail.x - s.tail.x)) / (s.subs.x * r.subs.y - s.subs.y * r.subs.x);

	// Plug the value of T2 to get T1
	T1 = (s.tail.x + s.subs.x * T2 - r.tail.x) / r.subs.x;

	// if there's intersection, change result to true
	if (T1 > 0 && T1 < 1 && T2 > 0 && T2 < 1)
	{
		inter.result = true;
		inter.pos = Vec2(r.tail.x + T1 * r.subs.x, r.tail.y + T1 * r.subs.y);
		inter.scalar = T1;
	}

	return inter;
}