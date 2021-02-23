#ifndef RAYCAST_H
#define RAYCAST_H

#include <GLM/glm.hpp>
#include "Coordinates.h"


struct RayHit {
	LocationXYZ blockToPlace, blockToBreak;
	glm::vec3 normal;
};


class Raycast {

public:
	static RayHit getBlockToPlace();
	static RayHit getBlockToBreak();
	static glm::vec3 getDirectBlock();

private:
	static LocationXYZ _getBlockPosition(const glm::vec3& pos);
	static glm::vec3 _getMouseRay();

};

#endif // RAYCAST_H
