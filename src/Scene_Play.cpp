#include "Scene_Play.hpp"
#include "GameEngine.hpp"

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
		case Left: break;
		case Up: break;
		case Down: break;
		case Right: break;
		case Paused: m_paused ? m_paused = false : m_paused = true; break;
		default:
			break;
		}
	}

	if (action.actionType() == KeyRelease)
	{
		switch (action.actionName())
		{
		case Left: break;
		case Up: break;
		case Down: break;
		case Right: break;
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
	if (m_paused)
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

}

void Scene_Play::sRender()
{

}