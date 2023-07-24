#pragma once

#include "Scene.hpp"

class Scene_Play : public Scene
{
protected:
	std::shared_ptr<Entity> m_player;
	std::vector<Vec2> m_points;
	std::vector<Vec2> m_linesSegment;
	std::vector<Vec2> m_intercepts;
	std::vector<float> m_angles;

	void init();
	void update();
	void onEnd();

	void sDoAction(const Action& action);
	void sLifespan();
	void sIntercept();
	void sRayCasting();
	void sMovement();
	void sRender();

	void spawnPlayer();
	void spawnBuild();

	void registerLine();

public:
	Scene_Play(GameEngine* gameEngine);

	void doAction(const Action& action);
};