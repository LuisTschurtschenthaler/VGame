#include <GLEW/GL/glew.h>
#include "Game.h"
#include "Camera.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderUtil.h"
#include "Window.h"
#include "TextRenderer.h"
#include "World.h"
#include "Player.h"
#include "Timer.h"
#include "Input.h"
#include "Crosshair.h"
#include "EventDispatcher.h"
#include "BlockEvents.h"


EventDispatcher Game::eventDispatcher = EventDispatcher();
bool Game::debugMode = false;
float Game::dayTime = 0;


Game::Game()
	: _HUDvisible(true) {

	_world = new World();

	_textRenderer = new TextRenderer();
	_textRenderer->init("./Resources/Fonts/font.ttf", 42);
	EventDispatcher::registerEvents(eventDispatcher);
}

Game::~Game() {
	delete _textRenderer;
	delete _world;
}


void Game::update() {
	/* ------------ TEMP ------------ */
	if(Input::isKeyPressed(GLFW_KEY_Q))
		dayTime -= 1;
	if(Input::isKeyPressed(GLFW_KEY_E))
		dayTime += 1;
	/* ------------ TEMP ------------ */
	dayTime += CoreEngine::gameTimer->getDeltaTime();

	_world->update();
	_textRenderer->update(&_world->getPlayer());
}

void Game::render() {
	RenderUtil::clearScreen();

	_world->draw();

	if(Input::isKeyPressedAndReleased(GLFW_KEY_F1))
		_HUDvisible = !_HUDvisible;

	if(_HUDvisible) {
		_textRenderer->draw();
		Crosshair::draw();
	}
}
