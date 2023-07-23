#pragma once

#include "Scene.hpp"
#include "Assets.hpp"

#include <memory>

typedef std::array<std::shared_ptr<Scene>, EnumScene::SCENE_COUNT> SceneArr;

class GameEngine
{
protected:
	sf::RenderWindow m_window;
	Assets m_assets;
	EnumScene m_currentScene;
	SceneArr m_sceneArr;
	size_t m_simulationSpeed = 1;
	bool m_running = true;

	void init(const std::string& path);
	void update();

	void sUserInput();

	std::shared_ptr<Scene> currentScene();

public:
	GameEngine(const std::string& path);

	void changeScene(EnumScene sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void quit();
	void run();

	sf::RenderWindow& window();
	Assets& assets();

	const Assets& assets() const;
	bool isRunning();
};