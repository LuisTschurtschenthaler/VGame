#ifndef RAYCAST_H
#define RAYCAST_H

#include <GLM/glm.hpp>
#include "Coordinates.h"


class Raycast {

public:
	static LocationXYZ getBlockToPlace();
	static LocationXYZ getBlockToBreak();
	static glm::vec3 getDirectBlock();

private:
	static LocationXYZ _getBlockPosition(const glm::vec3& pos);
	static glm::vec3 _getMouseRay();

};

#endif // RAYCAST_H
