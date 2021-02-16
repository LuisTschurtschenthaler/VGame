#include <GLM/glm.hpp>
#include "Player.h"
#include "Input.h"
#include "Window.h"
#include "World.h"
#include "CoreEngine.h"
#include "Constants.h"
#include "ChunkManager.h"
#include "FramerateCounter.h"
#include "Timer.h"
#include "Raycast.h"
#include "Game.h"
#include "EventDispatcher.h"
#include "EventTypes.h"
#include "Raycast.h"


void Player::_input() {
	_handleKeyboardInputs();
	_handleFOV();
	_handleMouseMove();
	_handleMouseButtons();
}

void Player::_handleKeyboardInputs() {
	float gravity = GRAVITY * (CoreEngine::gameTimer->getDeltaTime() / 20);
	float movementSpeed = ((isFlying) ? FLY_SPEED : WALK_SPEED) * CoreEngine::gameTimer->getDeltaTime();

	if(Input::isKeyPressed(KeyCode::KEY_LCTRL)) movementSpeed *= 1.25f;
	if(isSwimming) movementSpeed /= 1.25f;

	/* Input */
	glm::vec3 change(0.f);
	if(Input::isKeyPressed(KeyCode::KEY_W))
		change += camera->movingFront * movementSpeed;

	if(Input::isKeyPressed(KeyCode::KEY_A))
		change -= camera->right * movementSpeed;

	if(Input::isKeyPressed(KeyCode::KEY_S))
		change -= camera->movingFront * movementSpeed;

	if(Input::isKeyPressed(KeyCode::KEY_D))
		change += camera->right * movementSpeed;


	if(Input::isKeyPressed(KeyCode::KEY_LSHIFT))
		if(isFlying) change.y += -movementSpeed - 0.05;

	//if(Input::isKeyDoublePressed(KeyCode::KEY_SPACE)) else if
	if(Input::isKeyPressed(KeyCode::KEY_SPACE)) {
		if(isFlying)
			change.y += movementSpeed + 0.05;
		
		else if(isSwimming)
			change.y += movementSpeed;
		
		if(!isJumping && isOnGround) {
			isJumping = true;
			_jump = 0;
		}
	}

	/* Gravity */
	if(World::gravityEnabled && !isFlying) {
		if(isSwimming || isUnderwater) {
			change.y -= (gravity / 2);
			isJumping = false;
		}
		
		else if(isJumping) {
			change.y -= gravity;

			_jump += 0.125;
			change.y += std::sqrt((1 - _jump / JUMP_DURATION) * gravity * 0.175f); //(1 - _jump / JUMP_DURATION) * (movementSpeed + gravity) + 0.0125;

			if(_jump >= JUMP_DURATION)
				isJumping = false;

			isOnGround = false;
		}

		else change.y -= gravity;
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
		
		yaw += deltaPosition.x * MOUSE_SENSITIVITY;
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
	if(_mouseTimer->elapse() >= BLOCK_BREAK_DURATION) {
		// Break block
		if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_LEFT)) {
			Game::eventDispatcher.dispatchEvent(BLOCK_BREAK_EVENT, &World::getChunkManager());
			_mouseTimer->update();
		}

		// Place block
		else if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_RIGHT)) {
			Game::eventDispatcher.dispatchEvent(BLOCK_PLACE_EVENT, &World::getChunkManager());
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

	else if(camera->fov > FOV && camera->fov != FOV_ZOOM) {
			camera->fov -= FOV_SPRINT_STEPS;
		isSprinting = false;
	}

	else camera->fov = FOV;
}
