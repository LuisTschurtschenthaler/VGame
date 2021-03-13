#include <GLEW/GL/glew.h>
#include "AABB.h"
#include "World.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "Player.h"
#include "Block.h"
#include "Entity.h"


AABB::AABB(const glm::vec3& dimension)
	: dimensions(dimension) {
}

AABB::AABB()
	: dimensions({ 0.f }) {
}

AABB::~AABB() {
}


void AABB::update(const glm::vec3& pos) {
	min = pos;
	max = min + dimensions;
}

void AABB::update(const ChunkXZ& pos) {
	min = glm::vec3(pos.x, 0, pos.z);
	max = min + dimensions;
}


void AABB::handleCollision(Entity& entity, const glm::vec3& velocity) {
	const glm::vec3 dimensions = entity.box.dimensions;

	for(int x = entity.position.x - dimensions.x; x < entity.position.x + dimensions.x; x++)
	for(int y = entity.position.y - dimensions.y; y < entity.position.y + 0.15f; y++)
	for(int z = entity.position.z - dimensions.z; z < entity.position.z + dimensions.z; z++) {
		
		const Block& block = BlockManager::getBlock(World::getChunkManager().getBlockID({ x, y, z }));
		if(block.hasHitbox) {
			if(velocity.x > 0) {
				entity.position.x = x - dimensions.x;
				entity.velocity.x = 0;
				//player.velocity.x = -player.velocity.x;
			}
			else if(velocity.x < 0) {
				entity.position.x = x + dimensions.x + BLOCK_SIZE;
				entity.velocity.x = 0;
				//player.velocity.x = -player.velocity.x;
			}
		
			if(velocity.y > 0) {
				entity.position.y = y - 0.15f;
				entity.velocity.y = 0;
			}
			else if(velocity.y < 0) {
				entity.position.y = y + dimensions.y + BLOCK_SIZE;
				entity.velocity.y = 0;
				entity.isJumping = false;
			}
		
			if(velocity.z > 0) {
				entity.position.z = z - dimensions.z;
				entity.velocity.z = 0;
				//player.velocity.z = -player.velocity.z;
			}
			else if(velocity.z < 0) {
				entity.position.z = z + entity.box.dimensions.z + BLOCK_SIZE;
				entity.velocity.z = 0;
				//player.velocity.z = -player.velocity.z;
			}
		}
	}
}

bool AABB::isColliding(const Entity& entity) {
	for(int x = entity.position.x - entity.box.dimensions.x; x < entity.position.x + entity.box.dimensions.x; x++)
	for(int y = entity.position.y - entity.box.dimensions.y; y < entity.position.y + 0.15f; y++)
	for(int z = entity.position.z - entity.box.dimensions.z; z < entity.position.z + entity.box.dimensions.z; z++) {
		
		if(BlockManager::getBlock(World::getChunkManager().getBlockID({ x, y, z })).hasHitbox)
			return true;
	}
	return false;
}

void AABB::collision(Particle& particle, const float& size) {
	const glm::vec3 pos = particle.position,
					vel = particle.velocity;

	const Block& block = BlockManager::getBlock(World::getChunkManager().getBlockID({ 
		int(pos.x + vel.x), 
		int(pos.y + vel.y),
		int(pos.z + vel.z) 
	}));

	if(block.hasHitbox) {
		if(vel.x < 0) {
			particle.position.x = int(pos.x);
			particle.velocity.x = 0;
		}

		if(vel.z < 0) {
			particle.position.z = int(pos.z);
			particle.velocity.z = 0;
		}

		 if(vel.y < 0) {
			 particle.position.y = int(pos.y);
			 particle.velocity.y = 0;
			 particle.velocity.x *= 0.85f;
			 particle.velocity.z *= 0.85f;
		}
	}
}
