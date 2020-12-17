#ifndef RAYCAST_H
#define RAYCAST_H

#include "Coordinates.h"
class Camera;

class Raycast {

public:
	static BlockPositionXYZ rayCast(Camera& camera);

};

#endif // RAYCAST_H
