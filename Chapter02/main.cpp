#include "Game.h"

int main()
{
	Game game;

	bool bSuccess = game.Initialize();
	if (bSuccess)
	{
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}