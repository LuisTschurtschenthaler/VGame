#include "Window.h"
#include "GameEngine.h"	
#include <iostream>


int main() {
	Window window(1080, 1920, L"Bauers Endgame");

	GameEngine engine(window);
	engine.runGame();

	return 0;
}
