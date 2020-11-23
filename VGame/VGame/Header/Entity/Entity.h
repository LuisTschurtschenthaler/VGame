#ifndef ENTITY_H
#define ENTITY_H

#include <GLM/glm.hpp>
#include "AABB.h"

class ChunkManager;

class Entity {

protected:
	glm::vec3 position, rotation, velocity;
	AABB aabb;


public:
	//Entity();
	//~Entity();

	//void update();
	//bool isColliding(ChunkManager* chunkManager, const glm::vec3& pos);

};

#endif // ENTITY_H
