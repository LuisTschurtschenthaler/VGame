#ifndef CAMERA_H
#define CAMERA_H

#define FOV 90.f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.f

#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/euler_angles.hpp>
class CoreEngine;
class RenderEngine;
class Chunk;


class Camera {

private:
	glm::mat4 _view;
	glm::vec3 _position, _front, _right, _up, _worldUp;
	float _yaw, _pitch, _roll;
	bool _mouseLocked;


public:
	Camera(glm::vec3 position);
	~Camera() { }

	void handleMouseInputs(float mouseSensitivity);
	void handleKeyboardInputs(float movementSpeed);

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();

	void setPosition(glm::vec3 position) { _position = position; }
	glm::vec3 getPosition() const { return _position; }
	glm::vec3 getFront() const { return _front; }
	float getPitch() const { return _pitch; }
	float getYaw() const { return _yaw; }

private:
	void _updateVectors();

};

#endif // CAMERA_H
