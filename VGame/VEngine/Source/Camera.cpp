#include <cmath>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include "WorldConstants.h"
#include "Camera.h"
#include "Window.h"
#include "Player.h"


Camera::Camera(Player* player)
	: _player(player), _WORLD_UP(glm::vec3(0.f, 1.f, 0.f)) {

	fov = FOV_MAX;
	
	front = glm::vec3(0.f, 0.f, 1.f);
	right = glm::vec3(0.f);
	up = glm::vec3(0.f, 1.f, 0.f);
}

Camera::~Camera() {
}


void Camera::update() {
	position = _player->position;
	pitch = _player->pitch;
	yaw = _player->yaw;
};

glm::mat4 Camera::getView() {
	updateVectors();

	_view = glm::lookAt(position, position + front, up);
	return _view;
}

glm::mat4 Camera::getProjection() {
	glm::mat4 _projection = glm::mat4(1.f);
	_projection = glm::perspective(glm::radians(fov), Window::getAspect(), NEAR_PLANE, FAR_PLANE);

	return _projection;
}

glm::mat4 Camera::getProjectionView() {
	auto proj = getProjection();
	auto view = getView();

	return (proj * view);
}

void Camera::updateVectors() {
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(_front);
	right = glm::normalize(glm::cross(front, _WORLD_UP));
	up = glm::normalize(glm::cross(right, front));
}
