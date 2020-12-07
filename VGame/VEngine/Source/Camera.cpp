#include <cmath>
#include "Camera.h"
#include "CoreEngine.h"
#include "Input.h"
#include "Window.h"
#include "Timer.h"
#include "Chunk.h"
#include "World.h"
#include "ChunkSection.h"
#include "ChunkManager.h"
#include "Coordinates.h"
#include "Block.h"
#include "Player.h"


Camera::Camera(const glm::vec3& position)
	: _yaw(0.f), _pitch(0.f),
	_roll(0.f), _mouseLocked(false) {

	_position = position;
	_front = glm::vec3(0.f, 0.f, 1.f);
	_right = glm::vec3(0.f);
	_up = glm::vec3(0.f, 1.f, 0.f);
	_worldUp = glm::vec3(0.f, 1.f, 0.f);

	_velocity = glm::vec3(0.f);
	_speed = 1.f;
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

void Camera::update(ChunkManager* chunkManager, Player* player) {
	/*
	float cameraSpeed = 0.01f  * _speed;
	_position += _velocity * cameraSpeed;
	std::vector<BlockPositionXYZ>* adjacentBlocks = chunkManager->getAdjacentBlocks(_position);

	for(int i = 0; i < (*adjacentBlocks).size(); i++) {
		BlockPositionXYZ blockPosition = (*adjacentBlocks)[i];
		Block* block = BlockUtil::blocks[chunkManager->getBlock(blockPosition)];

		if(!block->hasHitbox)
			continue;

		glm::vec3 blockPos = glm::vec3(blockPosition.x, blockPosition.y, blockPosition.z);

		if(AABB::hit(blockPos, _position)) {
			std::cout << "HIT: " << block->name << std::endl;
			switch(i) {
				case 0: _position.x = blockPosition.x + BLOCK_SIZE * 1.5f; break;
				case 1: _position.x = blockPosition.x - BLOCK_SIZE * 1.5f; break;
				case 2: _position.y = blockPosition.y + BLOCK_SIZE * 1.5f; break;
				case 3: _position.y = blockPosition.y - BLOCK_SIZE * 1.5f; break;
				case 4: _position.z = blockPosition.z + BLOCK_SIZE * 1.5f; break;
				case 5: _position.z = blockPosition.z - BLOCK_SIZE * 1.5f; break;
				default: break;
			}
		}
	}*/
}

void Camera::handleMouseInputs(float mouseSensitivity) {
	glm::vec2 centerMousePosition = Window::getMouseCenterPosition();

	if(Input::isKeyPressed(KeyCode::KEY_ESCAPE)) {
		Input::setCursorVisible(true);
		_mouseLocked = false;
	}

	if(_mouseLocked) {
		glm::vec2 deltaPosition = Input::getMousePosition() - centerMousePosition;

		bool mouseMovedX = deltaPosition.x != 0;
		bool mouseMovedY = deltaPosition.y != 0;

		if(mouseMovedX) {
			deltaPosition.x *= mouseSensitivity;
			_yaw += deltaPosition.x;
		}

		if(mouseMovedY) {
			deltaPosition.y *= -mouseSensitivity;
			_pitch += deltaPosition.y;
		}

		if(mouseMovedX || mouseMovedY) {
			Input::setMousePosition(centerMousePosition);

			if(_pitch > 89.99f)
				_pitch = 89.99f;
			else if(_pitch < -89.99f)
				_pitch = -89.99f;

			if(_yaw > 360.f || _yaw < -360.f)
				_yaw = 0.f;
		}

		_updateVectors();
	}

	if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_LEFT)) {
		Input::setCursorVisible(false);
		Input::setMousePosition(centerMousePosition);
		_mouseLocked = true;
	}
}

void Camera::handleKeyboardInputs(ChunkManager* chunkManager, Player* player) {
	//glm::vec3 change(0.f);

	//if(Input::isKeyPressed(KeyCode::KEY_LSHIFT))
	//	_velocity *= 2.f;

	float velocity = 0.01f * Timer::getDelta();
		
	if(Input::isKeyPressed(KeyCode::KEY_W)) {
		glm::vec3 move = _toHorizontal(_front * velocity);
		if(!player->aabb->collision(chunkManager, player, _position + move))
			_position += move;
	}
	
	if(Input::isKeyPressed(KeyCode::KEY_A)) {
		glm::vec3 move = _toHorizontal(_right * velocity);
		if(!player->aabb->collision(chunkManager, player, _position - move))
			_position -= move;
	}

	if(Input::isKeyPressed(KeyCode::KEY_S)) {
		glm::vec3 move = _toHorizontal(_front * velocity);
		if(!player->aabb->collision(chunkManager, player, _position - move))
			_position -= move;
	}

	if(Input::isKeyPressed(KeyCode::KEY_D)) {
		glm::vec3 move = _toHorizontal(_right * velocity);
		if(!player->aabb->collision(chunkManager, player, _position + move))
			_position += move;
	}


	if(Input::isKeyPressed(KeyCode::KEY_SPACE)) {
		glm::vec3 move = glm::vec3(0.f, 1.5f, 0.f);
		if(!player->aabb->collision(chunkManager, player, _position + move))
			_position += move;
	}


	if(!player->aabb->collision(chunkManager, player, _position + glm::vec3(0.f, -GRAVITY, 0.f)))
		_position.y -= GRAVITY;

	//_position += change;
	//_position.x *= 0.95f;
	//_position.z *= 0.95f;
}

/*
void Camera::move(Direction direction, float movementSpeed) {
	float velocity = movementSpeed * Timer::getDelta();

	switch(direction) {
		case FORWARD:	_position += _toHorizontal(_front) * velocity; break;
		case BACKWARD:	_position -= _toHorizontal(_front) * velocity; break;
		case RIGHT:		_position += _toHorizontal(_right) * velocity; break;
		case LEFT:		_position -= _toHorizontal(_right) * velocity; break;
		case UP:		_position += _worldUp * velocity; break;
		case DOWN:		_position -= _worldUp * velocity; break;
	}
}
*/

glm::vec3 Camera::_toHorizontal(const glm::vec3& vec) {
	return { vec.x, 0.f, vec.z };
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
