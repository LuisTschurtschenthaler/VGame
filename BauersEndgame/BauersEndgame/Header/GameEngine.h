#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Window.h"
class Game;

class GameEngine {

private:
	Window _window;
	Game* _game;

public:
	GameEngine(Window& window);
	~GameEngine();

	void runGame();

private:
	void _init();

};

#endif // GAME_ENGINE_H
