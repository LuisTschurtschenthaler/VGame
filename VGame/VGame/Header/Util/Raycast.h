#ifndef RAYCAST_H
#define RAYCAST_H

#include <GLM/glm.hpp>
#include "Coordinates.h"

class Camera;
class ChunkManager;

class Raycast {

public:
	static glm::vec3 getBlockToBreak(Camera* camera, ChunkManager* chunkManager);
	static glm::vec3 getBlockToPlace(Camera* camera, ChunkManager* chunkManager);
	static BlockPositionXYZ getBlockPosition(const glm::vec3 pos);

private:
	static glm::vec3 _getMouseRay(Camera* camera, ChunkManager* chunkManager);

};

#endif // RAYCAST_H
