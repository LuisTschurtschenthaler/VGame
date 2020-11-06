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

	/* ---------------------------------------------------- */
	static bool isDay() { return (Game::dayTime >= 5.5 && Game::dayTime <= 18.3); }
	static float getDayTheta() { return static_cast<float>((Game::dayTime - SUNRISE_TIME) * PI); }
	static glm::vec3 getLightDirection() {
		float dayTheta = getDayTheta();

		if(isDay()) return glm::vec3(sin(PI) * cos(dayTheta), -sin(dayTheta), cos(PI) * cos(dayTheta));
			else return glm::vec3(-sin(PI) * cos(dayTheta), sin(dayTheta), -cos(PI) * cos(dayTheta));
	}
	/* ---------------------------------------------------- */

};

#endif // GAME_H
