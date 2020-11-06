#include "Game.h"
#include "Camera.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderUtil.h"
#include "Sky.h"
#include "Window.h"
#include "TerrainGenerator.h"
#include "TextRenderer.h"
#include "World.h"
#include "Player.h"
#include "Timer.h"


std::string Game::version = "V1.0.0";
float Game::dayTime = 0;


Game::Game() 
	: _coreEngine(nullptr), _textRenderer(nullptr), _sky(nullptr), 
	_player(nullptr), _world(nullptr) {

	dayTime = 6.2f;
}

Game::~Game() {
}


void Game::init() {
	_player = new Player();
	_world = new World(this, REALISTIC, "TEST"); // FLATLAND REALISTIC
	
	_sky = new Sky();
	_sky->init();
	
	_textRenderer = new TextRenderer();
	_textRenderer->init("./Resources/Fonts/font.ttf", 42);
}

void Game::update() {
	/* ------------ TEMP ------------ */
	if(Input::isKeyPressed(KeyCode::KEY_Q))
		dayTime -= 0.05f;
	if(Input::isKeyPressed(KeyCode::KEY_E))
		dayTime += 0.05f;
	/* ------------ TEMP ------------ */

	//dayTime += 0.01f;
	dayTime = (dayTime > 24) ? dayTime - 24 : (dayTime < 0) ? 24 : dayTime;

	_world->update();
	_textRenderer->update(_world->getChunkManager(), _player);
}

void Game::render() {
	RenderUtil::clearScreen();
	
	_world->draw();
	_sky->draw(*_player->camera);
	_textRenderer->draw();
}
