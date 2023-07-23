#pragma once

#include "GameEngine.hpp"
#include "Utils.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	std::ifstream fin(path);
	std::string temp;

	while (!fin.eof() && !fin.fail())
	{
		fin >> temp;
		if (temp == "Window")
		{
			fin >> temp;
			int width = stoi(Utils::split(temp, "="));

			fin >> temp;
			int height = stoi(Utils::split(temp, "="));

			fin >> temp;
			int frame = stoi(Utils::split(temp, "="));

			m_window.create(sf::VideoMode(width, height), "Window");
			m_window.setFramerateLimit(frame);
		}
	}
}

void GameEngine::run()
{
	while (isRunning())
	{
		sUserInput();
		update();
	}
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// check if key code is registered in key name enum
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) { continue; }

			// determine whether key type is press or release
			EnumKeyType key_type = (event.type == sf::Event::KeyPressed) ? KeyPress : KeyRelease;

			// compute the action based on action name and key type then pass to the scene
			currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), key_type));
		}
	}
}

// updating previous game state 
void GameEngine::update()
{
	currentScene()->update();
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneArr[m_currentScene];
}

void GameEngine::changeScene(EnumScene sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	m_sceneArr[sceneName] = scene;

	if (endCurrentScene) m_currentScene = sceneName;
}

void GameEngine::quit()
{
	m_running = false;
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

Assets& GameEngine::assets()
{
	return m_assets;
}

const Assets& GameEngine::assets() const
{
	return m_assets;
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}