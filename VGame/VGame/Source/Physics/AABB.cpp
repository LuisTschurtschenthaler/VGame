#include <cmath>
#include <GLEW/GL/glew.h>
#include "AABB.h"
#include "WorldConstants.h"
#include "ChunkManager.h"
#include "Player.h"


AABB::AABB() 
	: dimensions({ 0, 0, 0 }) {
}

AABB::AABB(const glm::vec3& dimension)
	: dimensions(dimension) {
}

AABB::~AABB() {
}


void AABB::collision(ChunkManager* chunkManager, Player& player, const glm::vec3& velocity) {
	for(int x = player.position.x - dimensions.x; x < player.position.x + dimensions.x; x++) {
		for(int y = player.position.y - dimensions.y; y < player.position.y + 0.5; y++) {
			for(int z = player.position.z - dimensions.z; z < player.position.z + dimensions.z; z++) {

				Block* block = BlockUtil::blocks[chunkManager->getBlock({ x, y, z + 1 })];
				player.isSwimming = (block->meshType == FLUID);

				if(block->hasHitbox) {
					if(velocity.x > 0)
						player.position.x = x - dimensions.x;
					else if(velocity.x < 0)
						player.position.x = x + dimensions.x + BLOCK_SIZE;
				
					if(velocity.y > 0) {
						player.position.y = y - dimensions.y;
						player.velocity.y = 0;
					}
					else if(velocity.y < 0) {
						player.position.y = y + dimensions.y + BLOCK_SIZE;
						player.velocity.y = 0;
						player.isOnGround = true;
					}
				
					if(velocity.z > 0)
						player.position.z = z - dimensions.z;
					else if(velocity.z < 0)
						player.position.z = z + dimensions.z + BLOCK_SIZE;
				}

			}
		}
	}
}

void AABB::update(glm::vec3 pos) {
	//pos.z += 1;

	min = pos;
	max = min + dimensions;
}
