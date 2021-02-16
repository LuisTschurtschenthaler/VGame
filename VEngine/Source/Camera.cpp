#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include "Constants.h"
#include "Camera.h"
#include "Window.h"
#include "Player.h"
#include "Random.h"


Camera::Camera(Entity* entity)
	: ENTITY(entity), _WORLD_UP(glm::vec3(0.f, 1.f, 0.f)) {

	fov = FOV;
	
	position = glm::vec3(0.f);
	front = glm::vec3(0.f, 0.f, -1.f);
	right = glm::vec3(0.f);
	up = glm::vec3(0.f, 1.f, 0.f);

	movingFront = front;
}

Camera::~Camera() {
}


void Camera::update() {
/*
	glm::vec3 _front;
	_front.x = cos(glm::radians(ENTITY->yaw)) * cos(glm::radians(ENTITY->pitch));
	_front.y = sin(glm::radians(ENTITY->pitch));
	_front.z = sin(glm::radians(ENTITY->yaw)) * cos(glm::radians(ENTITY->pitch));
*/

	glm::vec3 _front;
	_front.x = cos(glm::radians(ENTITY->yaw));
	_front.y = 0.f;
	_front.z = sin(glm::radians(ENTITY->yaw));
	movingFront = glm::normalize(_front);
	
	_front.x *= cos(glm::radians(ENTITY->pitch));
	_front.y = sin(glm::radians(ENTITY->pitch));
	_front.z *= cos(glm::radians(ENTITY->pitch));
	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, _WORLD_UP));
	up = glm::normalize(glm::cross(right, front));

	position = ENTITY->position;
};

glm::mat4 Camera::getView() {
	update();

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

glm::mat3 Camera::getBillboard() {
	return glm::mat3(glm::vec3(right.x, right.y, right.z), glm::vec3(up.x, up.y, up.z), glm::vec3(front.x, front.y, front.z));
}
