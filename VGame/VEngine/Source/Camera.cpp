#include "Camera.h"
#include "CoreEngine.h"
#include "Input.h"
#include "Window.h"
#include "Timer.h"
#include "Chunk.h"
#include <cmath>


Camera::Camera(glm::vec3 position)
	: _yaw(0.f), _pitch(0.f),
	_roll(0.f), _mouseLocked(false) {

	_position = position;
	_front = glm::vec3(0.f, 0.f, 1.f);
	_right = glm::vec3(0.f);
	_up = glm::vec3(0.f, 1.f, 0.f);
	_worldUp = glm::vec3(0.f, 1.f, 0.f);
}


glm::mat4 Camera::getView() {
	_updateVectors();
	_view = glm::lookAt(_position, _position + _front, _up);

	return _view;
}

glm::mat4 Camera::getProjection() {
	glm::mat4 proj = glm::mat4(1.f);
	proj = glm::perspective(glm::radians(FOV), Window::getAspect(), NEAR_PLANE, FAR_PLANE);

	return proj;
}

glm::mat4 Camera::getProjectionView() {
	return (getProjection() * getView());
}

void Camera::handleMouseInputs(float mouseSensitivity) {
	glm::vec2 centerMousePosition = Window::getMouseCenterPosition();

	if(Input::isKeyPressed(KeyCode::KEY_ESCAPE)) {
		Input::setCursorVisible(true);
		_mouseLocked = false;
	}

	if(_mouseLocked) {
		glm::vec2 deltaPosition = Input::getMousePosition() - centerMousePosition;

		bool rotationX = deltaPosition.x != 0;
		bool rotationY = deltaPosition.y != 0;

		if(rotationX) {
			deltaPosition.x *= mouseSensitivity;
			_yaw += deltaPosition.x;
		}

		if(rotationY) {
			deltaPosition.y *= -mouseSensitivity;
			_pitch += deltaPosition.y;
		}

		if(rotationX || rotationY) {
			Input::setMousePosition(centerMousePosition);

			if(_pitch > 89.9f)
				_pitch = 89.9f;
			else if(_pitch < -89.9f)
				_pitch = -89.9f;

			if(_yaw > 360.f || _yaw < -360.f)
				_yaw = 0.f;
		}
	}

	if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_LEFT)) {
		Input::setCursorVisible(false);
		Input::setMousePosition(centerMousePosition);
		_mouseLocked = true;
	}
}

void Camera::handleKeyboardInputs(float movementSpeed) {
	float velocity = movementSpeed * Timer::getDelta();

	if(Input::isKeyPressed(KeyCode::KEY_W))
		_position += _front * velocity;

	if(Input::isKeyPressed(KeyCode::KEY_A))
		_position -= _right * velocity;

	if(Input::isKeyPressed(KeyCode::KEY_S))
		_position -= _front * velocity;

	if(Input::isKeyPressed(KeyCode::KEY_D))
		_position += _right * velocity;


	if(Input::isKeyPressed(KeyCode::KEY_LSHIFT))
		_position -= _up * velocity;

	if(Input::isKeyPressed(KeyCode::KEY_SPACE))
		_position += _up * velocity;

	_updateVectors();
}


void Camera::_updateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	
	_front = glm::normalize(front);
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}