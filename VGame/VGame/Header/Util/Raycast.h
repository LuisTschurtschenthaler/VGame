#ifndef RAYCAST_H
#define RAYCAST_H

#include <GLM/glm.hpp>
#include "Coordinates.h"

class Camera;
class ChunkManager;

class Raycast {

public:
	static BlockPositionXYZ rayCast(Camera* camera, ChunkManager* chunkManager);

};

#endif // RAYCAST_H
