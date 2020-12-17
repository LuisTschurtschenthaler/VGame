#include <GLEW/GL/glew.h>
#include "AABB.h"
#include "WorldConstants.h"
#include "ChunkManager.h"
#include "Player.h"


AABB::AABB(const glm::vec3& dimension)
	: dimensions(dimension) {
}

AABB::~AABB() {
}


void AABB::update(glm::vec3 pos) {
	min = pos;
	max = min + dimensions;
}


void AABB::collision(ChunkManager* chunkManager, Player& player, const glm::vec3& velocity) {
	//Block* block1 = BlockUtil::blocks[chunkManager->getBlock({ player.position.x, player.position.y - 1, player.position.z })];
	//Block* block2 = BlockUtil::blocks[chunkManager->getBlock({ player.position.x, player.position.y, player.position.z })];
	player.isSwimming = false; //((block1->meshType == FLUID) || (block2->meshType == FLUID));

	for(int x = player.position.x - dimensions.x; x < player.position.x + dimensions.x; x++)
	for(int y = player.position.y - dimensions.y; y < player.position.y + 0.25; y++)
	for(int z = player.position.z - dimensions.z; z < player.position.z + dimensions.z; z++) {
		
		Block* block = BlockUtil::blocks[chunkManager->getBlock({ x, y, z })];
		if(block->hasHitbox) {
			if(velocity.x > 0) {
				player.position.x = x - dimensions.x;
				player.velocity.x = 0;
				//player.velocity.x = -player.velocity.x;
			}
			else if(velocity.x < 0) {
				player.position.x = x + dimensions.x + BLOCK_SIZE;
				//player.velocity.x = -player.velocity.x;
			}
		
			if(velocity.y > 0) {
				player.position.y = y - dimensions.y;
				player.velocity.y = 0;
			}
			else if(velocity.y < 0) {
				player.position.y = y + dimensions.y + BLOCK_SIZE;
				player.velocity.y = 0;
				player.isOnGround = true;
			}
		
			if(velocity.z > 0) {
				player.position.z = z - dimensions.z;
				player.velocity.z = 0;
				//player.velocity.z = -player.velocity.z;
			}
			else if(velocity.z < 0) {
				player.position.z = z + dimensions.z + BLOCK_SIZE;
				//player.velocity.z = -player.velocity.z;
			}
		}
	}

}
