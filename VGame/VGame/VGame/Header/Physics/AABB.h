#ifndef AABB_H
#define AABB_H

#include <GLM/glm.hpp>
#include "Coordinates.h"
#include "ParticleEmitter.h"

class Entity;
class Player;
class ChunkManager;


class AABB {
	
public:
	glm::vec3 min, max;
	glm::vec3 dimensions;


public:
	AABB(const glm::vec3& dimension);
	AABB();
	~AABB();

	void update(const glm::vec3& pos);
	void update(const ChunkXZ& pos);

	static void handleCollision(Entity& entity, const glm::vec3& velocity);
	static bool isColliding(const Entity& entity);
	static void collision(Particle& particle, const float& size);

};

#endif // AABB_H
