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
	for(int y = player.position.y - dimensions.y; y < player.position.y + 0.2; y++)
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
				player.position.y = y - 0.2;
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

glm::vec3 AABB::rayIntersectionWithBlock(glm::vec3 playerPos, glm::vec3 targetPos, glm::vec3 blockPos) {
	glm::vec3 direction = targetPos - playerPos;

	glm::vec3 scaling = (blockPos - playerPos) / direction;
	glm::vec3 scaled = playerPos + direction * scaling.x;
	
	if(direction.x > 0) {
		if(_isPositionInSquare({ scaled.z, scaled.y }, { blockPos.z, blockPos.y + 1.f, blockPos.z + 1.f, blockPos.y }))
			return glm::vec3(-1, 0, 0);
	}
	else if(direction.x <= 0) {
		if(_isPositionInSquare({ scaled.z, scaled.y }, { blockPos.z, blockPos.y + 1.f, blockPos.z + 1.f, blockPos.y }))
			return glm::vec3(1, 0, 0);
	}

	if(direction.y > 0) {
		if(_isPositionInSquare({ scaled.x, scaled.z }, { blockPos.x, blockPos.z + 1.f, blockPos.x + 1.f, blockPos.z }))
			return glm::vec3(0, -1, 0);
	}
	else if(direction.y <= 0) {
		if(_isPositionInSquare({ scaled.x, scaled.z }, { blockPos.x, blockPos.z + 1.f, blockPos.x + 1.f, blockPos.z }))
			return glm::vec3(0, 1, 0);
	}

	if(direction.z > 0) {
		if(_isPositionInSquare({ scaled.x, scaled.y }, { blockPos.x, blockPos.y + 1.f, blockPos.x + 1.f, blockPos.y }))
			return glm::vec3(0, 0, -1);
	}
	else if(direction.z <= 0) {
		if(_isPositionInSquare({ scaled.x, scaled.y }, { blockPos.x, blockPos.y + 1.f, blockPos.x + 1.f, blockPos.y }))
			return glm::vec3(0, 0, 1);
	}

	return glm::vec3(0.f);
}


bool AABB::_isPositionInSquare(const glm::vec2& position, const glm::vec4& square) {
	/* Square:
		x = top left x
		y = top left y
		z = bottom right x
		w = bottom right y
	*/

	return ((position.x >= square.x) && (position.x <= square.z) 
		&& ((position.y >= square.w) && (position.y <= square.y))
	);
}
