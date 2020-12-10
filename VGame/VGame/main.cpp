#include "CoreEngine.h"
#include "Game.h"


int main() {
	Game game;
	CoreEngine coreEngine(&game);
	coreEngine.createWindow(1980, 1020, false, "VGame " + Game::version);
	coreEngine.start();

	return 0;
}
