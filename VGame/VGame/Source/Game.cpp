#include <GLEW/GL/glew.h>
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
#include "Crosshair.h"

#include "EventDispatcher.h"
#include "BlockEvents.h"


EventDispatcher Game::eventDispatcher = EventDispatcher();
std::string Game::version = "V1.0.0";
float Game::dayTime = 0;


Game::Game() 
	: _coreEngine(nullptr), _textRenderer(nullptr), _sky(nullptr), 
	_player(nullptr), _world(nullptr) {
}

Game::~Game() {
}


void Game::init() {
	_player = new Player();
	_world = new World(this, REALISTIC, "TEST"); // FLATLAND REALISTIC
	_player->setToWorld(_world);

	_sky = new Sky();
	_sky->init();

	_textRenderer = new TextRenderer();
	_textRenderer->init("./Resources/Fonts/font.ttf", 42);

	EventDispatcher::registerEvents(eventDispatcher);
}

void Game::update() {
	/* ------------ TEMP ------------ */
	if(Input::isKeyPressed(KeyCode::KEY_Q))
		dayTime -= 1;
	if(Input::isKeyPressed(KeyCode::KEY_E))
		dayTime += 1;
	/* ------------ TEMP ------------ */

	dayTime += 0.01;
	dayTime = (dayTime > 360) ? dayTime - 360 : (dayTime < 0) ? 360 : dayTime;

	_world->update();
	_textRenderer->update(_world->getChunkManager(), _player);
}

void Game::render() {
	RenderUtil::clearScreen();
	Crosshair::draw();
	
	_world->prepareDraw();
	_world->draw();

	_sky->draw(_player);
	_textRenderer->draw();
}
