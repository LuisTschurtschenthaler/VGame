#ifndef AABB_H
#define AABB_H

#include <GLM/glm.hpp>

class Player;
class ChunkManager;


class AABB {
	
public:
	glm::vec3 min, max;
	glm::vec3 dimensions;

public:
	AABB(const glm::vec3& dimension);
	~AABB();

	void update(glm::vec3 pos);
	void collision(ChunkManager* chunkManager, Player& player, const glm::vec3& velocity);

};

#endif // AABB_H
