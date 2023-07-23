#pragma once

#include "Scene.hpp"

class Scene_Play : public Scene
{
protected:
	std::shared_ptr<Entity> m_player;

	void init();
	void update();
	void onEnd();

	void sDoAction(const Action& action);
	void sLifespan();
	void sCollision();
	void sAnimation();
	void sMovement();
	void sRender();

	void spawnPlayer();
	void spawnBuild();

public:
	Scene_Play(GameEngine* gameEngine);

	void doAction(const Action& action);
};