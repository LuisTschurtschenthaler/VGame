#ifndef ENTITY_H
#define ENTITY_H

#include <GLM/glm.hpp>
#include "AABB.h"


class Entity {

public:
	AABB box;

	glm::vec3 position, velocity;
	float yaw, pitch;
	bool isFlying, isOnGround, isJumping, isSwimming, isUnderwater, isSprinting;


public:
	Entity() { }
	~Entity() { }

};

#endif // ENTITY_H
