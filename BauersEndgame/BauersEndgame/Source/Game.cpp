#include "Game.h"
#include "Shop.h"


Game::Game() {

	_shop = new Shop();
}

Game::~Game() {

}


void Game::init() {
	_shop->init();
}

void Game::update() {
	_shop->open("Ws gibsn ht guits?? :D\n");
	std::string val = _shop->getValue();

}
