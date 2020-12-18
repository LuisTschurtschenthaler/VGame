#include "CoreEngine.h"
#include "Window.h"
#include "Game.h"

/*
	PLAYER:
		Mouse move			->		Look around
		W, A, S, D			->		Move
		Space				->		Jump/Fly up
	->	Left control		->		Sprint
		Left shift			->		Sneak/Fly down

		Left mouse button	->		Break block
		Right mouse button	->		Place block
	->	C					->		Zoom

	OTHER:
		Escape				->		Lose window focus

		F2					->		Take screenshot
		F3					->		Show/Hide game info
*/

int main() {
	Game game;

	Window::create(1980, 1020, false, true, "VGame " + Game::version);
	CoreEngine coreEngine(&game);
	coreEngine.start();

	return 0;
}
