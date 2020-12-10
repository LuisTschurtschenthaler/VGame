#include "Player.h"
#include "Input.h"
#include "Window.h"
#include "Timer.h"
#include "FramerateCounter.h"


void Player::_handleKeyboardInputs(ChunkManager* chunkManager) {
	//float speed = 0.01f * Timer::getDelta() / 10;
	float speed = WALK_SPEED + ((GRAVITY / 60) * (Timer::getDelta()/1000));
	std::cout << speed << std::endl;

	glm::vec3 change(0.f);

	if(Input::isKeyPressed(KeyCode::KEY_LSHIFT))
		speed *= 3.f;

	if(isSwimming)
		speed /= 1.5;


	if(Input::isKeyPressed(KeyCode::KEY_W))
		change += _toHorizontal(camera->front * speed);

	if(Input::isKeyPressed(KeyCode::KEY_A))
		change -= _toHorizontal(camera->right * speed);

	if(Input::isKeyPressed(KeyCode::KEY_S))
		change -= _toHorizontal(camera->front * speed);

	if(Input::isKeyPressed(KeyCode::KEY_D))
		change += _toHorizontal(camera->right * speed);


	if(Input::isKeyPressed(KeyCode::KEY_LCTRL))
		change.y -= speed;

	if(Input::isKeyPressed(KeyCode::KEY_SPACE)) {
		if(isFlying)
			change.y += speed + 1;

		else if(isSwimming)
			change.y += WALK_SPEED * speed;
	}

	velocity += change;
}


void Player::_handleMouseInputs() {
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
			deltaPosition.x *= MOUSE_SENSITIVITY;
			yaw += deltaPosition.x;
		}

		if(mouseMovedY) {
			deltaPosition.y *= -MOUSE_SENSITIVITY;
			pitch += deltaPosition.y;
		}

		if(mouseMovedX || mouseMovedY) {
			Input::setMousePosition(centerMousePosition);

			if(pitch > 89.99f)
				pitch = 89.99f;
			else if(pitch < -89.99f)
				pitch = -89.99f;

			if(yaw > 360.f || yaw < -360.f)
				yaw = 0.f;
		}
		camera->update();
	}

	if(Input::isMousebuttonPressed(KeyCode::MOUSE_BUTTON_LEFT)) {
		Input::setCursorVisible(false);
		Input::setMousePosition(centerMousePosition);
		_mouseLocked = true;
	}
}


glm::vec3 Player::_toHorizontal(const glm::vec3& vec) {
	return { vec.x, 0.f, vec.z };
}
