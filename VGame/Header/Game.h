#ifndef GAME_H
#define GAME_H

#include <GLM/glm.hpp>
#include "World.h"

class CoreEngine;
class EventDispatcher;
class TextRenderer;

enum GameState {
	GAME_LOADING,
	GAME_ACTIVE,
	GAME_MENU
};


class Game {

public:
	static EventDispatcher eventDispatcher;
	static GameState gameState;
	static bool debugMode;
	static float dayTime;

private:
	CoreEngine* _coreEngine;
	TextRenderer* _textRenderer;
	World* _world;


public:
	Game();
	~Game();
	
	void update();
	void render();

	void setCoreEngine(CoreEngine* coreEngine) { _coreEngine = coreEngine; }
	World& getWorld() const { return *_world; }

};

#endif // GAME_H
