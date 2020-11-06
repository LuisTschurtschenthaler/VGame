#include "CoreEngine.h"
#include "Game.h"

/*
SPHERE
sqrt((float) (x - CHUNK_SIZE / 2)*(x - CHUNK_SIZE / 2) + (y - CHUNK_SIZE / 2)*(y - CHUNK_SIZE / 2) + (z - CHUNK_SIZE / 2)*(z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2

OTHER
https://www.brawl.com/threads/67313/

*/

int main() {
	Game game;
	CoreEngine coreEngine(&game);
	coreEngine.createWindow(1980, 1020, "VGame " + Game::version);
	coreEngine.start();

	return 0;
}
