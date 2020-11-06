#include "AABB.h"
#include <cmath>

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max)
	: min(min), max(max) {
}

AABB::~AABB() {
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
