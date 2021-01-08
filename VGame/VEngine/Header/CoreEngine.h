#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <string>

class Timer;
class Game;


class CoreEngine {

public:
	static Timer* gameTimer;

private:
	Game* _game;
	bool _isRunning;


public:
	CoreEngine(Game* game);
	~CoreEngine();

	void start();
	void stop();

private:
	void _run();

};

#endif // CORE_ENGINE_H