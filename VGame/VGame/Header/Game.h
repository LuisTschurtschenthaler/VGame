#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <GLM/glm.hpp>
#include "World.h"
#include "WorldConstants.h"

class CoreEngine;
class Camera;
class Sky;
class Player;
class TextRenderer;

/*
enum GameState {
	GAME_ACTIVE,
	GAME_MENU
};*/

class Game {

public:
	static std::string version;
	static float dayTime;

private:
	CoreEngine* _coreEngine;
	TextRenderer* _textRenderer;
	Player* _player;
	World* _world;
	Sky* _sky;


public:
	Game();
	~Game();
	
	void init();
	void update();
	void render();

	void setCoreEngine(CoreEngine* coreEngine) { _coreEngine = coreEngine; }
	Player* getPlayer() const { return _player; }
	World* getWorld() const { return _world; }
	Sky* getSky() const { return _sky; }

	static bool isDay() { return (Game::dayTime >= 0 && Game::dayTime <= 180); }

};

#endif // GAME_H
