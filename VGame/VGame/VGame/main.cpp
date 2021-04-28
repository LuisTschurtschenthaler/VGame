#include "CoreEngine.h"
#include "Window.h"
#include "Game.h"


int main() {
	Window::create("VGame - V1.0", false, true);

	Game game;
	CoreEngine coreEngine(&game);
	coreEngine.start();

	return 0;
}
