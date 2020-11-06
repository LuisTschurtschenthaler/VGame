#ifndef PLAYER_H
#define PLAYER_H

#define WALK_SPEED 0.05f
#define FLY_SPEED 0.075f
#define MOUSE_SENSITIVITY 0.1f

#include <GLM/glm.hpp>
#include "Camera.h"
#include "AABB.h"

class Player {

public:
	Camera* camera;
	glm::vec3 lastChunkPosition;

private:
	bool _flying;
	const float _cardinalPointRange;
	AABB* _boundingBox;


public:
	Player();
	~Player();

	void update();
	void updatePhysics();
	std::string yawToCardinalPoint();

	void setSpawnPoint(glm::vec3 spawnPoint);
	glm::vec3 getPosition() const { return camera->getPosition(); }
	glm::vec3 getFront() const { return camera->getFront(); }
	
	AABB getBouningBox() const { return { _boundingBox->min + camera->getPosition(), _boundingBox->max + camera->getPosition() }; }

private:
	bool _isInRange(float yaw, int numberToCheckPos, int numberToCheckNeg = 0);

};

#endif // PLAYER_H
