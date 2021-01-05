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
	bool _mouseLocked;
	float _jump;


public:
	Player();
	~Player();

	void update();

private:
	void _input();
	void _handleKeyboardInputs();
	void _handleMouseMove();
	void _handleMouseButtons();
	void _handleFOV();
	glm::vec3 _toHorizontal(const glm::vec3& vec);

};

#endif // PLAYER_H
