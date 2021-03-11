#include <GLEW/GL/glew.h>
#include "AABB.h"
#include "World.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "Player.h"
#include "Block.h"


AABB::AABB(const glm::vec3& dimension)
	: dimensions(dimension) {
}

AABB::AABB()
	: dimensions({ 0.f }) {
}

AABB::~AABB() {
}


void AABB::update(glm::vec3 pos) {
	min = pos;
	max = min + dimensions;
}

void AABB::update(ChunkXZ pos) {
	min = glm::vec3(pos.x, 0, pos.z);
	max = min + dimensions;
}


void AABB::collision(Player& player, const glm::vec3& velocity) {
	for(int x = player.position.x - dimensions.x; x < player.position.x + dimensions.x; x++)
	for(int y = player.position.y - dimensions.y; y < player.position.y + 0.15f; y++)
	for(int z = player.position.z - dimensions.z; z < player.position.z + dimensions.z; z++) {
		
		Block* block = BlockManager::blocks[World::getChunkManager().getBlockID({ x, y, z })];
		if(block->hasHitbox) {
			if(velocity.x > 0) {
				player.position.x = x - dimensions.x;
				player.velocity.x = 0;
				//player.velocity.x = -player.velocity.x;
			}
			else if(velocity.x < 0) {
				player.position.x = x + dimensions.x + BLOCK_SIZE;
				player.velocity.x = 0;
				//player.velocity.x = -player.velocity.x;
			}
		
			if(velocity.y > 0) {
				player.position.y = y - 0.15f;
				player.velocity.y = 0;
			}
			else if(velocity.y < 0) {
				player.position.y = y + dimensions.y + BLOCK_SIZE;
				player.velocity.y = 0;
				player.isJumping = false;
			}
		
			if(velocity.z > 0) {
				player.position.z = z - dimensions.z;
				player.velocity.z = 0;
				//player.velocity.z = -player.velocity.z;
			}
			else if(velocity.z < 0) {
				player.position.z = z + dimensions.z + BLOCK_SIZE;
				player.velocity.z = 0;
				//player.velocity.z = -player.velocity.z;
			}
		}
	}
}

void AABB::collision(Particle& particle, const float& size) {
	const glm::vec3 pos = particle.position,
					vel = particle.velocity;

	const Block* block = BlockManager::blocks[World::getChunkManager().getBlockID({ 
		int(pos.x + vel.x), 
		int(pos.y + vel.y),
		int(pos.z + vel.z) 
	})];

	if(block->hasHitbox) {
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
