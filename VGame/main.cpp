#include "CoreEngine.h"
#include "Window.h"
#include "Game.h"

/*
	Mouse move			->	Look around
	W, A, S, D			->	Move
	Space/Double space 	->	Jump/Fly up
	Left control		->	Sprint
	Left shift			->	(Sneak)/Fly down

	Left mouse button	->	Break block
	Right mouse button	->	Place block

	Escape				->	Lose window focus
	F2					->	Take screenshot
	F3					->	Show/Hide game info
	F11					->	Toggle fullscreen
*/

int main() {
	Window::create(false, true, "VGame");

	Game game;
	CoreEngine coreEngine(&game);
	coreEngine.start();

	return 0;
}
