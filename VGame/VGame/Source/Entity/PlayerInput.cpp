#include "Player.h"
#include "Input.h"
#include "Window.h"
#include "World.h"
#include "CoreEngine.h"
#include "WorldConstants.h"
#include "FramerateCounter.h"
#include "Timer.h"


void Player::_handleKeyboardInputs(ChunkManager* chunkManager) {
	float movementSpeed = WALK_SPEED + ((GRAVITY / MAX_FPS) * CoreEngine::gameTimer->getDeltaTime());
	float gravity = GRAVITY * CoreEngine::gameTimer->getDeltaTime() / 17;

	if(Input::isKeyPressed(KeyCode::KEY_LSHIFT)) movementSpeed *= 1.5f;
	if(isSwimming) movementSpeed /= 1.2f;

	/* Input */
	glm::vec3 change(0.f);
	if(Input::isKeyPressed(KeyCode::KEY_W))
		change += _toHorizontal(camera->front * movementSpeed);

	if(Input::isKeyPressed(KeyCode::KEY_A))
		change -= _toHorizontal(camera->right * movementSpeed);

	if(Input::isKeyPressed(KeyCode::KEY_S))
		change -= _toHorizontal(camera->front * movementSpeed);

	if(Input::isKeyPressed(KeyCode::KEY_D))
		change += _toHorizontal(camera->right * movementSpeed);


	if(Input::isKeyPressed(KeyCode::KEY_LCTRL))
		if(isFlying && !isOnGround)
			change.y -= movementSpeed;

	//if(Input::isKeyDoublePressed(KeyCode::KEY_SPACE)) else if
	if(Input::isKeyPressed(KeyCode::KEY_SPACE)) {
		/*
		if(isFlying)
			change.y += movementSpeed + 1;

		else if(isSwimming)
			change.y += movementSpeed + 0.3f;
		*/
		if(!isJumping && isOnGround) {
			jumpTimer->update();
			isJumping = true;
		}
	}


	/* Gravity */
	if(World::gravityEnabled) {
		if(!isFlying) {
			/*if(!isOnGround && !isSwimming)
				velocity.y -= gravity;

			if(isSwimming)
				velocity.y -= gravity + 0.05;
			isOnGround = false;*/

			change.y -= gravity;

			if(isJumping) {
				change.y += (1 - jumpTimer->elapse() / JUMP_DURATION) * 0.165;

				if(jumpTimer->elapse() >= JUMP_DURATION);
					isJumping = false;
				isOnGround = false;
			}
		}
	}

	velocity += change;
}


void Player::_handleMouseInputs() {
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


glm::vec3 Player::_toHorizontal(const glm::vec3& vec) {
	return { vec.x, 0.f, vec.z };
}
