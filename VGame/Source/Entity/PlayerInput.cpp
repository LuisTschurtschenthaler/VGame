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
	_handleMouseInputs();
}

void Player::_handleKeyboardInputs() {
	if(Input::isKeyDoublePressed(GLFW_KEY_SPACE)) {
		isFlying = !isFlying;
		if(!isFlying) velocity.y = 0;
	}

	float gravity = 0.5f * CoreEngine::gameTimer->getDeltaTime();
	float movementSpeed = ((isFlying) ? FLY_SPEED : WALK_SPEED) * CoreEngine::gameTimer->getDeltaTime();

	if(Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL)) movementSpeed *= 1.25f;
	if(isSwimming) movementSpeed /= 1.25f;

	/* Input */
	glm::vec3 change(0.f);
	if(Input::isKeyPressed(GLFW_KEY_W))
		change += camera->movingFront * movementSpeed;

	if(Input::isKeyPressed(GLFW_KEY_A))
		change -= camera->right * movementSpeed * 0.8f;

	if(Input::isKeyPressed(GLFW_KEY_S))
		change -= camera->movingFront * movementSpeed;

	if(Input::isKeyPressed(GLFW_KEY_D))
		change += camera->right * movementSpeed * 0.8f;


	if(Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
		if(isFlying) change.y += -(movementSpeed / 1.25f);


	if(Input::isKeyPressed(GLFW_KEY_SPACE)) {
		if(isFlying)
			change.y += (movementSpeed / 1.25f);
		
		else if(isSwimming)
			change.y += movementSpeed + 0.01f;
		
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
			change.y += std::sqrt((1 - _jump) * gravity * 0.175f); // 0.145f

			if(_jump >= JUMP_DURATION)
				isJumping = false;
		}

		else change.y -= gravity;
	}

	if(change.x != 0.f || change.z != 0.f)
		Game::eventDispatcher.dispatchEvent(EventType::PLAYER_MOVE_EVENT, this);

	velocity += change;
}

void Player::_handleMouseMove() {
	glm::vec2 centerMousePosition = Window::getMouseCenterPosition();

	if(Input::mouseLocked && Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		Input::setCursorVisible(true);
		Input::mouseLocked = false;
		Input::enabled = false;
	}
	else if(!Input::mouseLocked && Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		Input::setCursorVisible(false);
		Input::setMousePosition(centerMousePosition);
		Input::mouseLocked = true;
		Input::enabled = true;
	}

	if(Input::mouseLocked) {
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

void Player::_handleMouseInputs() {
	// Handle mouse wheel
	int mouseWheelScroll = Input::getMouseScroll(ScrollType::VERTICAL);
	if(mouseWheelScroll != 0) {
		_selectedBlock += mouseWheelScroll;
		_selectedBlock = (_selectedBlock % (TOTAL_BLOCKS - 1));

		if(_selectedBlock < 1)
			_selectedBlock = (mouseWheelScroll < 1) ? (TOTAL_BLOCKS - 1) : 1;

		std::cout << BlockManager::getBlock(_selectedBlock).name << std::endl;
	}

	// Select block
	if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
		LocationXYZ blockLocation = Raycast::getBlockToBreak().blockToBreak;
		_selectedBlock = World::getChunkManager().getBlockID(blockLocation);
	}


	// Break block
	if(Input::isMouseButtonPressedAndReleased(GLFW_MOUSE_BUTTON_LEFT)
	   || (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && _mouseTimer->elapse() >= BLOCK_BREAK_DURATION)) {

		LocationXYZ blockBreakLocation = Raycast::getBlockToBreak().blockToBreak;
		Game::eventDispatcher.dispatchEvent(BLOCK_BREAK_EVENT, blockBreakLocation);
		_mouseTimer->update();
	}
	// Place block
	else if(Input::isMouseButtonPressedAndReleased(GLFW_MOUSE_BUTTON_RIGHT)
		|| (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && _mouseTimer->elapse() >= BLOCK_BREAK_DURATION)) {

		RayHit rayHit = Raycast::getBlockToPlace();
		
		const Block& block = BlockManager::getBlock(_selectedBlock);
		BlockID selectedBlockID = static_cast<BlockID>(_selectedBlock);

		Game::eventDispatcher.dispatchEvent(BLOCK_PLACE_EVENT, rayHit.blockToPlace, selectedBlockID);
		_mouseTimer->update();
	}
}

void Player::_handleFOV() {
	if(Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL) &&
	   Input::isKeyPressed(GLFW_KEY_W)) {

		if(camera->fov < FOV_SPRINT)
			camera->fov += FOV_SPRINT_STEPS;
		isSprinting = true;
	}

	else if(camera->fov > FOV && camera->fov != FOV_ZOOM) {
			camera->fov -= FOV_SPRINT_STEPS;
		isSprinting = false;
	}
	else camera->fov = FOV;


	static bool keyC = false;
	if(keyC && !Input::isKeyPressed(GLFW_KEY_C)) {
		camera->fov = FOV;
		keyC = false;
	}
	else if(Input::isKeyPressed(GLFW_KEY_C)) {
		camera->fov = 40.f;
		keyC = true;
	}
}
