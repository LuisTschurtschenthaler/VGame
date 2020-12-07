#ifndef AABB_H
#define AABB_H

#include <GLM/glm.hpp>
#include "Vertex.h"
class Player;
class ChunkManager;


// AABB = Axis-Aligned Bounding Boxes
// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
class AABB {
	
public:
	glm::vec3 min, max;
	glm::vec3 dimension;

public:
	AABB();
	AABB(const glm::vec3& dimension);
	~AABB();

	static bool hit(glm::vec3& block, glm::vec3& player);

	void draw();
	void update(glm::vec3 pos);
	
	bool collision(ChunkManager* chunkManager, Player* player, glm::vec3 pos);
	bool hitsBlock(AABB& block);

	bool intersectsWith(const AABB& other) const;
	bool touches(const AABB& other, float delta) const;
	bool isPointInsideAABB(const glm::vec3& point, const AABB& other) const;

};

#endif // AABB_H
