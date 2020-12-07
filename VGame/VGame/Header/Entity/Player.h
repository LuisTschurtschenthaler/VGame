#ifndef PLAYER_H
#define PLAYER_H

#define WALK_SPEED 0.05f
#define FLY_SPEED 0.075f
#define JUMP_TIME 1.f
#define MOUSE_SENSITIVITY 0.1f

#include <GLM/glm.hpp>
#include "Camera.h"
#include "AABB.h"

class ChunkManager;

class Player {

public:
	Camera* camera;
	AABB* aabb;
	glm::vec3 lastChunkPosition;
	bool isFlying, isFalling, isJumping, isUnderwater;

private:
	const float _cardinalPointRange;


public:
	Player();
	~Player();

	void update(ChunkManager* chunkManager);
	void keyoardInput();

	std::string yawToCardinalPoint();

	void setSpawnPoint(glm::vec3 spawnPoint);
	glm::vec3 getPosition() const { return camera->getPosition(); }
	glm::vec3 getFront() const { return camera->getFront(); }

private:
	bool _isInRange(float yaw, int numberToCheckPos, int numberToCheckNeg = 0);

};

#endif // PLAYER_H
