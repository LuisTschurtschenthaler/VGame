#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <string>

class Timer;
class Game;
class Window;


class CoreEngine {

public:
	static Timer* gameTimer;

private:
	Game* _game;
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