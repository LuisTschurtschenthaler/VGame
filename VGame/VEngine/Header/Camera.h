#ifndef CAMERA_H
#define CAMERA_H

#include <GLM/glm.hpp>

class CoreEngine;
class RenderEngine;
class Chunk;
class Player;
class ChunkManager;


class Camera {

public:
	float fov;
	glm::vec3 position;
	glm::vec3 front, right, up;

private:
	const Player* _PLAYER;
	const glm::vec3 _WORLD_UP;
	glm::mat4 _projection;
	glm::mat4 _view;


public:
	Camera(Player* player);
	~Camera();

	void update();

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();

};

#endif // CAMERA_H
