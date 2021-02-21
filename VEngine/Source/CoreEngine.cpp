#include "CoreEngine.h"
#include "RenderUtil.h"
#include "Window.h"
#include "Input.h"
#include "Game.h"
#include "Log.h"
#include "Random.h"
#include "TextRenderer.h"
#include "FramerateCounter.h"
#include "Timer.h"
#include "Screenshot.h"
#include "LoadingScreen.h"


Timer* CoreEngine::gameTimer = new Timer();


CoreEngine::CoreEngine(Game* game)
	: _game(game), _isRunning(false) {

	_game->setCoreEngine(this);
}

CoreEngine::~CoreEngine() {
	Window::dispose();
}


void CoreEngine::start() {
	if(_isRunning) return;
	_run();
}

void CoreEngine::stop() {
	if(!_isRunning) return;
	_isRunning = false;
}

void CoreEngine::_run() {
	_isRunning = true;

	RenderUtil::initGraphics();
	Log::setFile("./VGame.log");
	Input::init();
	Random::init();
	FramerateCounter::init();
	LoadingScreen::init();

	while(_isRunning) {
		if(Window::shouldClose())
			stop();

		gameTimer->update();
		FramerateCounter::update();

		_game->update();
		Input::update();

		Window::handle();
		Screenshot::handle();
		TextRenderer::handle();

		_game->render();
		Window::render();
	}
}
