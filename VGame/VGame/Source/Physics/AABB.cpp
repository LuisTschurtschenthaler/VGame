#include <cmath>
#include <GLEW/GL/glew.h>
#include "AABB.h"
#include "WorldConstants.h"
#include "ChunkManager.h"


AABB::AABB() 
	: dimension({ 0, 0, 0 }) {
}

AABB::AABB(const glm::vec3& dimension)
	: dimension(dimension) {
}

AABB::~AABB() {
}


void AABB::update(const glm::vec3& pos) {
	min = pos;
	max = min + dimension;
}

bool AABB::collision(ChunkManager* chunkManager, const glm::vec3& pos) {
	update(pos - (dimension / 2.f));
	BlockPositionXYZ playerPosition = { int(std::floor(pos.x)), int(std::floor(pos.y)), int(std::floor(pos.z)) };

	for(int x = min.x; x < max.x; x++) {
		for(int y = min.y - 1.5; y < max.y; y++) {
			for(int z = min.z; z < max.z; z++) {
				BlockPositionXYZ blockPos(x, y, z);
				Block* block = BlockUtil::blocks[chunkManager->getBlock(blockPos)];

				if(!block->hasHitbox) continue;
				AABB blockAABB = BlockUtil::getBlockAABB(blockPos);

				if(hitsBlock(blockAABB))
					return true;
			}
		}
	}
	return false;
}

bool AABB::hitsBlock(AABB& block) {
	return (min.x <= block.max.x && max.x >= block.min.x)
		&& (min.y <= block.max.y && max.y >= block.min.y)
		&& (min.z <= block.max.z && max.z >= block.min.z);
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
