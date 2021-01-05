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


void AABB::collision(Player& player, const glm::vec3& velocity) {
	//Block* block1 = BlockUtil::blocks[chunkManager->getBlockID({ player.position.x, player.position.y - 1, player.position.z })];
	//Block* block2 = BlockUtil::blocks[chunkManager->getBlockID({ player.position.x, player.position.y, player.position.z })];
	player.isSwimming = false; //((block1->meshType == FLUID) || (block2->meshType == FLUID));

	for(int x = player.position.x - dimensions.x; x < player.position.x + dimensions.x; x++)
	for(int y = player.position.y - dimensions.y; y < player.position.y + 0.15; y++)
	for(int z = player.position.z - dimensions.z; z < player.position.z + dimensions.z; z++) {
		
		Block* block = BlockUtil::blocks[World::getChunkManager().getBlockID({ x, y, z })];
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
				player.position.y = y - 0.15;
				player.velocity.y = 0;
				player.isOnGround = false;
			}
			else if(velocity.y < 0) {
				player.position.y = y + dimensions.y + BLOCK_SIZE;
				player.velocity.y = 0;
				player.isJumping = false;
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
	glm::vec3 scaling2 = ((blockPos + 1.f) - playerPos) / direction;
	
	if(direction.x > 0) {
		glm::vec3 scaled = playerPos + direction * scaling.x;

		if(_isPositionInSquare({ scaled.z, scaled.y }, { blockPos.z, blockPos.y + 1, blockPos.z + 1, blockPos.y }))
			return glm::vec3(-1, 0, 0);
	}
	if(direction.x <= 0) {
		glm::vec3 scaled = playerPos + direction * scaling2.x;

		if(_isPositionInSquare({ scaled.z, scaled.y }, { blockPos.z, blockPos.y + 1, blockPos.z + 1, blockPos.y }))
			return glm::vec3(1, 0, 0);
	}

	if(direction.y > 0) {
		glm::vec3 scaled = playerPos + direction * scaling.y;

		if(_isPositionInSquare({ scaled.x, scaled.z }, { blockPos.x, blockPos.z + 1, blockPos.x + 1, blockPos.z }))
			return glm::vec3(0, -1, 0);
	}
	if(direction.y <= 0) {
		glm::vec3 scaled = playerPos + direction * scaling2.y;

		if(_isPositionInSquare({ scaled.x, scaled.z }, { blockPos.x, blockPos.z + 1, blockPos.x + 1, blockPos.z }))
			return glm::vec3(0, 1, 0);
	}

	if(direction.z > 0) {
		glm::vec3 scaled = playerPos + direction * scaling.z;

		if(_isPositionInSquare({ scaled.x, scaled.y }, { blockPos.x, blockPos.y + 1, blockPos.x + 1, blockPos.y }))
			return glm::vec3(0, 0, -1);
	}
	if(direction.z <= 0) {
		glm::vec3 scaled = playerPos + direction * scaling2.z;

		if(_isPositionInSquare({ scaled.x, scaled.y }, { blockPos.x, blockPos.y + 1, blockPos.x + 1, blockPos.y }))
			return glm::vec3(0, 0, 1);
	}

	return glm::vec3(0);
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