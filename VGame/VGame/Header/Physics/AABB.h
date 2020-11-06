#ifndef AABB_H
#define AABB_H

#include <GLM/glm.hpp>

// AABB = Axis-Aligned Bounding Boxes
// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
class AABB {

public:
	glm::vec3 min, max;


public:
	AABB(const glm::vec3& min, const glm::vec3& max);
	~AABB();

	bool intersectsWith(const AABB& other) const;
	bool touches(const AABB& other, float delta) const;
	bool isPointInsideAABB(const glm::vec3& point, const AABB& other) const;

};

#endif // AABB_H
