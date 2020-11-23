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

/*
void Camera::handleKeyboardInputs(ChunkManager* chunkManager, Player* player, float movementSpeed) {
	_velocity.x *= 0.7f;
	_velocity.y  = 0.f;
	_velocity.z *= 0.7f;

	if(player->isFlying)
		_velocity.y = GRAVITY * 0.6f;
	else if(player->isSwimming)
		_velocity -= GRAVITY * 0.3f;
	else
		_velocity -= GRAVITY;

	//if(Input::isKeyPressed(KeyCode::KEY_LSHIFT))
	//	_velocity *= 2.f;

	glm::vec3 updateVelocity = movementSpeed * _velocity * float(Timer::getDelta());
	//std::cout << updateVelocity.x << " " << updateVelocity.y << " " << updateVelocity.z << std::endl;
	
	_position.y += updateVelocity.y;
	if(player->aabb->collision(chunkManager, _position)) {
		updateVelocity.y = 0;
		_position.y -= updateVelocity.y;
	}

	
	if(Input::isKeyPressed(KeyCode::KEY_W)) {
		_position -= _front * updateVelocity;
		if(player->aabb->collision(chunkManager, _position))
			_position += _front * updateVelocity;
	}
	
	if(Input::isKeyPressed(KeyCode::KEY_A)) {
		_position += _right * updateVelocity;
		if(player->aabb->collision(chunkManager, _position))
			_position -= _right * updateVelocity;
	}

	if(Input::isKeyPressed(KeyCode::KEY_S)) {
		_position += _front * updateVelocity;
		if(player->aabb->collision(chunkManager, _position))
			_position -= _front * updateVelocity;
	}

	if(Input::isKeyPressed(KeyCode::KEY_D)) {
		_position -= _right * updateVelocity;
		if(player->aabb->collision(chunkManager, _position))
			_position += _right * updateVelocity;
	}

	if(Input::isKeyPressed(KeyCode::KEY_SPACE))
		_position -= _worldUp * updateVelocity * glm::vec3(1, GRAVITY*2, 1);

	if(Input::isKeyPressed(KeyCode::KEY_C))
		positionChange -= _worldUp * velocity;

	if(Input::isKeyPressed(KeyCode::KEY_SPACE))
		positionChange += _worldUp * velocity;
}

void Camera::updatePhysics(ChunkManager* chunkManager, AABB* box) {
	box->update(_position);
 	BlockPositionXYZ playerPosition = { int(_position.x), int(_position.y), int(_position.z) };

	for(int x = box->min.x; x < box->max.x; x++) {
		for(int y = box->min.y; y < box->max.y; y++) {
			for(int z = box->min.z; z < box->max.z; z++) {
				BlockPositionXYZ blockPos(x, y, z);
				Block* block = BlockUtil::blocks[chunkManager->getBlock(blockPos)];

				if(block->meshType != MeshType::SOLID) continue;
				AABB blockAABB = BlockUtil::getBlockAABB(blockPos);

				if(box->hitsBlock(blockAABB))
					std::cout << "HIT: " << std::endl;

			}
		}
	}

	/*
	std::vector<BlockPositionXYZ> adjacents = {
		{ -1,  0,  0 },
		{  1,  0,  0 },
		{  0, -1,  0 },
		{  0,  1,  0 },
		{  0,  0, -1 },
		{  0,  0,  1 }
	};

	for(int i = 0; i < 6; i++) {
		BlockPositionXYZ toCheck = playerPosition + adjacents[i];
		//std::cout << toCheck << std::endl;
		Block* block = BlockUtil::blocks[chunkManager->getBlock(toCheck)];

		if(block->meshType != MeshType::SOLID) continue;
		AABB blockAABB = BlockUtil::getBlockAABB(toCheck);

		if(playerAABB->hitsBlock(blockAABB)) {
			switch(i) {
				case 0: _position.x = toCheck.x + BLOCK_SIZE * 1.5; break;
				case 1: _position.x = toCheck.x - BLOCK_SIZE * 0.5; break;
				case 2: _position.y = toCheck.y + BLOCK_SIZE * 1.5; break;
				case 3: _position.y = toCheck.y - BLOCK_SIZE * 0.5; break;
				case 4: _position.z = toCheck.z + BLOCK_SIZE * 1.5; break;
				case 5: _position.z = toCheck.z - BLOCK_SIZE * 0.5; break;
				default: break;
			}
		}
	} /*
}*/


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
