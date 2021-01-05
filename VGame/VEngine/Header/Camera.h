#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"

class CoreEngine;
class RenderEngine;
class Chunk;
class Player;
class ChunkManager;


class Camera {

public:
	const Entity* ENTITY;

	float fov;
	glm::vec3 position;
	glm::vec3 front, right, up;

private:
	const glm::vec3 _WORLD_UP;
	glm::mat4 _projection, _view;


public:
	Camera(Entity* entity);
	~Camera();

	void update();

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();

};

#endif // CAMERA_H
