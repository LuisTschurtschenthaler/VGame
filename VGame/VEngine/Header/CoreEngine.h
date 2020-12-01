#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <string>
class Game;
class Window;
class Settings;

class CoreEngine {

private:
	Game* _game;
	Settings* _settings;
	bool _isRunning;


public:
	CoreEngine(Game* game);
	~CoreEngine();

	void createWindow(int width, int height, bool fullscreen, const std::string& windowTitle);
	void start();
	void stop();

private:
	void _run();

};

#endif // CORE_ENGINE_H