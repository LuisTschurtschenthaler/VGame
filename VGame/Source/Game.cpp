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
bool Game::debugMode = false;
float Game::dayTime = 0;


Game::Game() {
	_world = new World();

	_textRenderer = new TextRenderer();
	_textRenderer->init("./Resources/Fonts/font.ttf", 42);

	EventDispatcher::registerEvents(eventDispatcher);
}

Game::~Game() {
	delete _world;
	delete _textRenderer;
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
	_textRenderer->update(&_world->getPlayer());
}

void Game::render() {
	RenderUtil::clearScreen();
	//Crosshair::draw();
	
	if(debugMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		_world->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else _world->draw();

	_textRenderer->draw();
}
