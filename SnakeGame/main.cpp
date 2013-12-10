#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Game.hpp"

int main()
{
	Game game = Game();
	game.Run();

	return 0;
}