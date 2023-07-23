#include "GameEngine.hpp"
#include "Scene_Play.hpp"

int main(int argc, char* argv[])
{
	GameEngine game("config.txt");
	game.changeScene(PlayScene, std::make_shared<Scene_Play>(&game), true);
	game.run();

	return 0;
}