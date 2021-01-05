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
	AABB();
	~AABB();

	void update(glm::vec3 pos);
	void collision(Player& player, const glm::vec3& velocity);

	static glm::vec3 rayIntersectionWithBlock(glm::vec3 playerPos, glm::vec3 targetPos, glm::vec3 blockPos);

private:
	static bool _isPositionInSquare(const glm::vec2& position, const glm::vec4& square);

};

#endif // AABB_H
