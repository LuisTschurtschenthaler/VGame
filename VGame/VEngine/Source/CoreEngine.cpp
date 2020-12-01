#include "CoreEngine.h"
#include "RenderUtil.h"
#include "Window.h"
#include "Input.h"
#include "Timer.h"
#include "Game.h"
#include "Log.h"
#include "Screenshot.h"
#include "TextRenderer.h"
#include "Random.h"
#include "FramerateCounter.h"


CoreEngine::CoreEngine(Game* game)
	: _game(game), _settings(nullptr), _isRunning(false) {

	_game->setCoreEngine(this);
}

CoreEngine::~CoreEngine() {
	Window::dispose();
}


void CoreEngine::createWindow(int width, int height, bool fullscreen, const std::string& windowTitle) {
	Window::create(width, height, fullscreen, windowTitle);
	RenderUtil::initGraphics();
	Log::setFile("./VGame.log");
	Input::setup();
	Timer::setup();
	Random::init();
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
	_game->init();

	while(_isRunning) {
		if(Window::shouldClose())
			stop();

		Timer::update();
		FramerateCounter::update();
		Input::update();
		_game->update();

		Screenshot::handle();
		TextRenderer::handle();

		_game->render();
		Window::render();
	}
}
