#include "GameEngine.h"
#include "Game.h"


GameEngine::GameEngine(Window& window)
	: _window(window) {

	_game = new Game();
}

GameEngine::~GameEngine() {
}


void GameEngine::runGame() {
	_init();

	do {
		_game->update();


	} while(!_game->stop);
}


void GameEngine::_init() {
	_window.init();
	_game->init();
}
