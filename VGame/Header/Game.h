#ifndef GAME_H
#define GAME_H

#include <GLM/glm.hpp>
#include "World.h"

class CoreEngine;
class EventDispatcher;
class TextRenderer;


class Game {

public:
	static EventDispatcher eventDispatcher;
	static bool debugMode;
	static float dayTime;

private:
	CoreEngine* _coreEngine;
	TextRenderer* _textRenderer;
	World* _world;
	bool _HUDvisible;


public:
	Game();
	~Game();
	
	void update();
	void render();

	void setCoreEngine(CoreEngine* coreEngine) { _coreEngine = coreEngine; }
	World& getWorld() const { return *_world; }

};

#endif // GAME_H
