#ifndef PLAYER_H
#define PLAYER_H

#include <GLM/glm.hpp>
#include "Entity.h"
#include "Camera.h"

class Timer;


class Player 
	: public Entity {

public:
	Camera* camera;

private:
	Timer* _mouseTimer;
	glm::vec3 _lastChunkPosition;
	float _jump;


public:
	Player();
	~Player();

	void update();

private:
	void _input();
	void _handleKeyboardInputs();
	void _handleFOV();
	void _handleMouseMove();
	void _handleMouseButtons();

};

#endif // PLAYER_H
