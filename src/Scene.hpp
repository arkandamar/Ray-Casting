#pragma once

#include "Action.hpp"
#include "EntityManager.hpp"

class GameEngine;

enum EnumScene { Default = -1, PlayScene, SCENE_COUNT = PlayScene + 1 };

typedef std::map<int, EnumAction> ActionMap;

class Scene
{
protected:
	size_t m_currentFrame = 0;
	GameEngine* m_game = nullptr;
	EntityManager m_entityManager;
	ActionMap m_actionMap;
	bool m_paused = false;
	bool m_hasEnded = false;

	virtual void onEnd() = 0;
	void setPaused(bool paused);

public:
	Scene();
	Scene(GameEngine* gameEngine);

	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	virtual void doAction(const Action& action) = 0;
	void simulate(const size_t frames);
	void registerAction(int inputKey, EnumAction actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	bool hasEnded() const;
	const ActionMap& getActionMap() const;
	void drawLine(const Vec2& p1, const Vec2& p2);
};