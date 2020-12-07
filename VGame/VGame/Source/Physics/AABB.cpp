#include <cmath>
#include <GLEW/GL/glew.h>
#include "AABB.h"
#include "WorldConstants.h"
#include "ChunkManager.h"
#include "Player.h"


AABB::AABB() 
	: dimension({ 0, 0, 0 }) {
}

AABB::AABB(const glm::vec3& dimension)
	: dimension(dimension) {
}

AABB::~AABB() {
}


bool AABB::hit(glm::vec3& block, glm::vec3& player) {
	return (block.x < player.x + BLOCK_SIZE && block.x + BLOCK_SIZE > player.x - BLOCK_SIZE)
		&& (block.y < player.y + BLOCK_SIZE && block.y + BLOCK_SIZE > player.y - BLOCK_SIZE)
		&& (block.z < player.z + BLOCK_SIZE && block.z + BLOCK_SIZE > player.z - BLOCK_SIZE);
}				   


void AABB::update(glm::vec3 pos) {
	pos.z += 1;

	min = pos;
	max = pos + dimension;
}

bool AABB::collision(ChunkManager* chunkManager, Player* player, glm::vec3 pos) {
	update(pos);

	for(int x = min.x; x < max.x; x++) {
		for(int y = min.y - 1.75; y < max.y; y++) {
			for(int z = min.z; z < max.z; z++) {
				glm::vec3 blockPos(x, y, z);
				BlockPositionXYZ blockPos2(x, y, z);

				Block* block = BlockUtil::blocks[chunkManager->getBlock(blockPos2)];
				if(!block->hasHitbox) continue;

				glm::vec3 p1 = pos + glm::vec3(0.5, 0, 0.5);
				glm::vec3 p2 = pos - glm::vec3(0.5, 0, 0.5);

				if(hit(blockPos, p1) || hit(blockPos, p2)) {
				//if(player->aabb->hitsBlock(BlockUtil::getBlockAABB(blockPos2))) {
					std::cout << "HIT: " << block->name << std::endl;
					return true;
				}
			}
		}
	}
	return false;
}

bool AABB::hitsBlock(AABB& block) {
	return (block.min.x < min.x + BLOCK_SIZE && block.max.x + BLOCK_SIZE > max.x - BLOCK_SIZE)
		&& (block.min.y < min.y + BLOCK_SIZE && block.max.y + BLOCK_SIZE > max.y - BLOCK_SIZE)
		&& (block.min.z < min.z + BLOCK_SIZE && block.max.z + BLOCK_SIZE > max.z - BLOCK_SIZE);
	/*
	return (min.x < block.max.x && max.x > block.min.x)
		&& (min.y < block.max.y && max.y > block.min.y)
		&& (min.z < block.max.z && max.z > block.min.z);
		*/
}

void AABB::draw() {
	/*	 
	     4 + - - - - - + 5
		   | \         | \
		   |   \       |   \
		   |   6 + - - - - - + 7  max
		   |     |     |     |
		   |     |     |     |
	min  0 + - - | - - + 1   |	     ^ Y-Axis
			 \   |       \   |       |
			   \ |         \ |    \  |
			   2 + - - - - - + 3   \ |
							 Z-Axis \|
				  X-Axis <-----------+

	glBegin(GL_LINE);

	glVertex3f(min.x, min.y, min.z); // 0
	glVertex3f(max.x, min.y, min.z); // 1
	glVertex3f(min.x, min.y, max.z); // 2
	glVertex3f(max.x, min.y, max.z); // 3

	glVertex3f(min.x, max.y, min.z); // 4
	glVertex3f(max.x, max.y, min.z); // 5
	glVertex3f(min.x, max.y, max.z); // 6
	glVertex3f(max.x, max.y, max.z); // 7

	glEnd();
	*/
}

bool AABB::intersectsWith(const AABB& other) const {
	return (min.x < other.max.x && max.x > other.min.x)
		&& (min.y < other.max.y && max.y > other.min.y)
		&& (min.z < other.max.z && max.z > other.min.z);
}

bool AABB::touches(const AABB& other, float delta) const {
	return (min.x <= (other.max.x + delta) && max.x >= (other.min.x - delta))
		&& (min.y <= (other.max.y + delta) && max.y >= (other.min.y - delta))
		&& (min.z <= (other.max.z + delta) && max.z >= (other.min.z - delta));
}

bool AABB::isPointInsideAABB(const glm::vec3& point, const AABB& other) const {
	return (point.x >= other.min.x && point.x <= other.max.x)
		&& (point.y >= other.min.y && point.y <= other.max.y)
		&& (point.z >= other.min.z && point.z <= other.max.z);
}
