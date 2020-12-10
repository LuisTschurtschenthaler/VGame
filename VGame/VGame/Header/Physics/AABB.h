#ifndef AABB_H
#define AABB_H

#include <GLM/glm.hpp>
#include "Vertex.h"
#include "Coordinates.h"

class Player;
class ChunkManager;


// AABB = Axis-Aligned Bounding Boxes
// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
class AABB {
	
public:
	glm::vec3 min, max;
	glm::vec3 dimensions;

public:
	AABB();
	AABB(const glm::vec3& dimension);
	~AABB();

	void collision(ChunkManager* chunkManager, Player& player, const glm::vec3& velocity);
	void update(glm::vec3 pos);

};

#endif // AABB_H
