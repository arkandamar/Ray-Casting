#pragma once

#include "Scene.hpp"

struct Intersect 
{
	bool result;
	Vec2 pos;
	float scalar;
};

struct LineSegment
{
	size_t id;
	Vec2 tail;
	Vec2 head;
	Vec2 subs;
};


class Scene_Play : public Scene
{
protected:
	std::shared_ptr<Entity> m_player;
	std::vector<Vec2> m_points;
	std::vector<LineSegment> m_linesSegment;
	std::vector<LineSegment> m_rays;
	std::vector<Vec2> m_intercepts;
	std::vector<float> m_angles;

	void init();
	void update();
	void onEnd();

	void sDoAction(const Action& action);
	void sLifespan();
	void sIntercept();
	void sPoint();
	void sRayCasting();
	void sMovement();
	void sRender();

	void spawnPlayer();
	void spawnBuild();

	void registerLine();

	Intersect LineIntersect(LineSegment line1, LineSegment line2) const;
public:
	Scene_Play(GameEngine* gameEngine);

	void doAction(const Action& action);
};