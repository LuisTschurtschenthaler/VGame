#include <GLM/glm.hpp>
#include <GLM/geometric.hpp>
#include "Player.h"
#include "Input.h"
#include "Window.h"
#include "World.h"
#include "CoreEngine.h"
#include "WorldConstants.h"
#include "ChunkManager.h"
#include "FramerateCounter.h"
#include "Timer.h"
#include "Raycast.h"


void Player::_handleKeyboardInputs(ChunkManager* chunkManager) {
	float gravity = GRAVITY * CoreEngine::gameTimer->getDeltaTime() / 20;
	float movementSpeed = ((isFlying) ? FLY_SPEED : WALK_SPEED) * CoreEngine::gameTimer->getDeltaTime();

	if(Input::isKeyPressed(KeyCode::KEY_LCTRL)) movementSpeed *= 1.25f;
	if(isSwimming) movementSpeed /= 1.25f;

	/* Input */
	glm::vec3 change(0.f);
	if(Input::isKeyPressed(KeyCode::KEY_W))
		change += _toHorizontal(camera->front) * movementSpeed;

	if(Input::isKeyPressed(KeyCode::KEY_A))
		change -= _toHorizontal(camera->right) * movementSpeed;

	if(Input::isKeyPressed(KeyCode::KEY_S))
		change -= _toHorizontal(camera->front) * movementSpeed;

	if(Input::isKeyPressed(KeyCode::KEY_D))
		change += _toHorizontal(camera->right) * movementSpeed;


	if(Input::isKeyPressed(KeyCode::KEY_LSHIFT))
		if(isFlying)
			change.y += -movementSpeed - 0.05;

	//if(Input::isKeyDoublePressed(KeyCode::KEY_SPACE)) else if
	if(Input::isKeyPressed(KeyCode::KEY_SPACE)) {
		if(isFlying)
			change.y += movementSpeed + 0.05;
		/*
		else if(isSwimming)
			change.y += movementSpeed + 0.3f;
		*/
		if(!isJumping && isOnGround) {
			isJumping = true;
			_jump = 0;
		}
	}


	/* Gravity */
	if(World::gravityEnabled) {
		if(!isFlying) {
			change.y -= gravity;
						
			if(isSwimming)
				velocity.y -= 0.025;
			
			if(isJumping) {
				_jump += 0.15;
				change.y += (1 - _jump / JUMP_DURATION) * (movementSpeed + gravity) + 0.0165;

				if(_jump >= JUMP_DURATION)
					isJumping = false;

				isOnGround = false;
			}
		}
	}

	velocity += change;
}

void Player::_handleMouseMove() {
	glm::vec2 centerMousePosition = Window::getMouseCenterPosition();

	if(Input::isKeyPressed(KeyCode::KEY_ESCAPE)) {
		Input::setCursorVisible(true);
		_mouseLocked = false;
	}
	else if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_LEFT)) {
		Input::setCursorVisible(false);
		Input::setMousePosition(centerMousePosition);
		_mouseLocked = true;
	}

	if(_mouseLocked) {
		glm::vec2 deltaPosition = Input::getMousePosition() - centerMousePosition;
		yaw   += deltaPosition.x *  MOUSE_SENSITIVITY;
		pitch += deltaPosition.y * -MOUSE_SENSITIVITY;

		if(pitch > 89.99f)
			pitch = 89.99f;
		else if(pitch < -89.99f)
			pitch = -89.99f;

		if(yaw > 360.f || yaw < -360.f)
			yaw = 0.f;
		
		camera->update();
		Input::setMousePosition(centerMousePosition);
	}
}

void Player::_handleMouseButtons() {
	if(_mouseTimer->elapse() >= MOUSE_TIMEOUT) {
		// Break block
		if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_LEFT)) {
			BlockPositionXYZ blockPosition = Raycast::getBlockPosition(Raycast::getBlockToBreak(camera, _chunkManager));
			
			if(blockPosition.x == -1.f) return;
			_chunkManager->removeBlock(blockPosition);
			_chunkManager->recreateMesh(blockPosition);
			_mouseTimer->update();
		}

		// Place block
		else if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_RIGHT)) {
			BlockPositionXYZ blockPosition = Raycast::getBlockPosition(Raycast::getBlockToPlace(camera, _chunkManager));

			if(blockPosition.x == -1.f) return;
			_chunkManager->placeBlock(blockPosition, BlockType::CACTUS);
			_chunkManager->recreateMesh(blockPosition);
			_mouseTimer->update();
		}

	}
}

void Player::_handleFOV() {
	if(Input::isKeyPressed(KeyCode::KEY_LCTRL) &&
	   Input::isKeyPressed(KeyCode::KEY_W)) {

		if(camera->fov < FOV_SPRINT)
			camera->fov += FOV_SPRINT_STEPS;

		isSprinting = true;
	}

	else {
		if(camera->fov > FOV)
			camera->fov -= FOV_SPRINT_STEPS;

		isSprinting = false;
	}
}


glm::vec3 Player::_toHorizontal(const glm::vec3& vec) {
	return { vec.x, 0.f, vec.z };
}
